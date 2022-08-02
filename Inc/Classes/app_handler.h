
#ifndef _APP_HANDLER_H_
#define _APP_HANDLER_H_

#include <string>
#include <memory>

#ifndef GTEST
#include "stm32f4xx_hal.h"
#endif

#include "app.h"
#include "input_parser.h"
#include "robust_suspension.h"
#include "inverted_pendulum.h"

using std::unique_ptr;
using std::make_unique;
using std::tuple;

class AppHandler {
public:
	AppHandler();
	void initialize(shared_ptr<InputParser> parser, shared_ptr<RobustSuspension> robust_suspension, 
		shared_ptr<InvertedPendulum> inverted_pendulum);
	string processData(const string &data);


protected:
	shared_ptr<InputParser> parser;
	shared_ptr<RobustSuspension> robust_suspension;
	shared_ptr<InvertedPendulum> inverted_pendulum;

	int pendulum_counter  =   0;

};

#endif /* _APP_HANDLER_H_ */








