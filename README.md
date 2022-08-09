## Embedded Controller on ARM Cortex-M4 (STM32F4)

- [Embedded Controller on ARM Cortex-M4 (STM32F4)](#embedded-controller-on-arm-cortex-m4-stm32f4)
	- [1. Applications](#1-applications)
	- [2. Project Summary](#2-project-summary)
	- [3. Project Structure](#3-project-structure)
	- [4. Tools & Libraries](#4-tools--libraries)
	- [5. Unit Tests](#5-unit-tests)
	- [6. Google Tests](#6-google-tests)
	- [7. Dependencies](#7-dependencies)
	- [8. Demonstration Video](#8-demonstration-video)
	- [9. References](#9-references)

### 1. Applications
~~~
1. Robust Suspension
2. Inverted Pendulum on a Cart
~~~

### 2. Project Summary
This project consists of 2 applications. It runs on an ARM-Cortex M4 processor and shows the results on a PC. The communication between the ARM processor and the PC is via TCP/IP. I've used the LwIP library on the ARM processor for TCP/IP communication. Other libraries that I've used in this project are FreeRTOS as the Real-Time Operating System and Google-Test for Testing the Embedded Application.

The first application is about "Robust Suspension," it shows how a Robust Controller can handle a suspension system and create a significant result compare to "Passive Suspension." I've created a PC interface Application named "Embedded App Interface" (Link below in the Dependencies section) that simulates the Suspension System and send the measurements to the "Embedded Controller." After the ARM Processor calculates the result and sends the results back to the "Embedded App Interface," it shows the result. The "Embedded App Interface" also can show a "Passive Suspension" result so that the user can compare both the "Active Suspension" and the "Passive Suspension." In the "Embedded App Interface" user can set "Road Profile" parameters like "Bump Length" and "Bump Height."

The second application is about "Inverted Pendulum on a Cart." It is a classic application in "Control Theory." I've found an example in the "Data Driven Science & Engineering, Machine Learning, Dynamical Systems, and Control" Book and implemented it on an ARM processor. Like the first application, it uses the "Embedded App Interface." The user can choose the "Angle" of the "Inverted Pendulum" then run the application and see the results.
You can see the "Demonstration Video" on Youtube for more details. (Link below)



### 3. Project Structure

	├── Drivers                     # Low Level Drivers
	    ├── BSP                     # Board Support Package
	    ├── CMSIS                   # CMSIS Drivers
	    ├── STM32F4xx_HAL_Driver    # STM32 HAL Drivers
	├── gtests                      # Google Tests
	├── Inc                         # Include Directory
	├── MDK-ARM                     # Keil IDE Support Files
	├── Middlewares                 # Middleware like LwIP and FreeRTOS
	├── Src                         # Project Source Directory
	    ├── Classes                 # Project Specific Classes
	    ├── Math                    # Linear Algebra Tools
	    ├── Tests                   # Unit Tests
	├── Utilities                   # CPU Specific Files


### 4. Tools & Libraries
~~~
Processor : STM32F429
IDE 	  : Keil uVision 5
RTOS 	  : FreeRTOS
Language  : C++ 14
Library   : LwIP for Socket Programming
Test 	  : Google Test
~~~

### 5. Unit Tests
There are multiple Unit-Tests for each application. The Unit-Tests must run on the Hardware. You can enable them by defining "Proprocessor Symbols" in Keil IDE. <br />
First go to: Project -> Options for Target -> C/C++ -> Preprocessor Symbols, then Add "TEST_ENV" to the "Define" section. <br />
Compile the application and then "Debug" the project on the Hardware.
You can see the result of the Unit-Tests in the "Debug (printf) Viewer." If the "ITM" is not enabled, you have to enable it. <br />
First go to: Project -> Options for Target -> Debug -> by choosing "ST-Link Debugger" select "Settings" -> Trace, make sure to check the "Trace Enable" and in the "ITM Stimulus Ports" check the "0" bit.


### 6. Google Tests
There is no need to run the Integration-Tests on the Hardware. You can run them on your PC. First, make sure to install "google-tests" on your PC.
Navigate to the "gtests" directory and then create a build directory, then navigate to it. After that follow these commands:
```
$ cmake -G "MinGW Makefiles"  ..
$ make
$ .\gTestEmbeddedController.exe
```

### 7. Dependencies
To see the results of the Embedded Device, I used "[Embedded App Interface](https://github.com/Afshari/Embedded_App_Interface)." For more information, you can see the Github page. 

### 8. Demonstration Video
You can watch the [Demonstration Video](https://www.youtube.com/watch?v=Km_E-MVQTGk) on the Youtube.

### 9. References
~~~
1. H∞ control of active vehicle suspensions with actuator time delay, 
	Haiping Du, Nong Zhang, 2007
2. Output-Feedback-Based H∞ Control for Vehicle Suspension Systems 
	With Control Delay, Hongyi Li, Xingjian Jing, and Hamid Reza Karimi, 2014
3. Data Driven Science & Engineering, Machine Learning, Dynamical Systems, and Control
	Steven L. Brunton, J. Nathan Kutz
~~~