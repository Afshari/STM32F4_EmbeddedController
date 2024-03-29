

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "ethernetif.h"
#include "lwip/netif.h"
#include "lwip/tcpip.h"
#include "app_ethernet.h"
#include "tcp_server.h"
#include "Tests/tests_runner.h"
#include "app_handler.h"
#include "FreeRTOS.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static GPIO_InitTypeDef  GPIO_InitStruct;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);
static void StartThread(void const * argument);

static string network_response_handler(struct pbuf *p);
NetworkDataHandler network_data_handler;
AppHandler app_handler;
shared_ptr<InputParser> parser;
shared_ptr<InvertedPendulum> inverted_pendulum;
shared_ptr<RobustSuspension> robust_suspension;
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{

  HAL_Init();

  /* Configure the system clock to 180 MHz */
  SystemClock_Config();

  
#ifdef TEST_ENV
	TestsRunner testsRunner;
	testsRunner.run();
#endif
	
	parser = make_shared<InputParser>();
	inverted_pendulum = make_shared<InvertedPendulum>();
	robust_suspension = make_shared<RobustSuspension>();
	app_handler.initialize(parser, robust_suspension, inverted_pendulum);
	
    /* Init thread */
#if defined(__GNUC__)
  osThreadDef(Start, StartThread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 10);
#else
  osThreadDef(Start, StartThread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 2);
#endif
  
  osThreadCreate (osThread(Start), NULL);
  
  /* Start scheduler */
  osKernelStart();

  while (1) {
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 180000000
  *            HCLK(Hz)                       = 180000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 8
  *            PLL_N                          = 360
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  
  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 360;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
   while(1) {};
  }
  
  if(HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
   while(1) {};
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
   while(1) {};
  }
}



/**
  * @brief  Start Thread 
  * @param  argument not used
  * @retval None
  */
static void StartThread(void const * argument)
{
  /* Create tcp_ip stack thread */
  tcpip_init(NULL, NULL);
  
  /* Initialize the LwIP stack */
  Netif_Config();
	tcp_server_init(network_response_handler);
  
  
  /* Notify user about the network interface config */
  // User_notification(&gnetif);


  for( ;; )
  {
    /* Delete the Init Thread */ 
    osThreadTerminate(NULL);
  }
}

static string network_response_handler(struct pbuf *p) {

	string str( (char*) p->payload );
	str = str.substr(0, p->len);
	printf("Recv: %s\r\n", str.c_str());
	NetworkDataStatus status = network_data_handler.process(str);
	if(status == NetworkDataStatus::Complete) {

		string result = app_handler.processData(network_data_handler.getProcessedData());
		// printf("Result: %s", result.c_str());

		pbuf_free(p);
		p = pbuf_alloc(PBUF_RAW, result.length(), PBUF_POOL);
		p->payload = (void *) result.c_str();	
	}

	return "";
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Turn LED1 off */
  BSP_LED_Off(LED1);
  while(1)
  {
  }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

