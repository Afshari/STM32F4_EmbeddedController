
#ifndef _TESTS_RUNNER_H_
#define _TESTS_RUNNER_H_

#include "Tests/test_input_parser.h"
#include "Tests/test_network_data_handler.h"
#include "Tests/test_linear_algebra.h"
#include "Tests/test_inverted_pendulum.h"
#include "Tests/test_robust_suspension.h"


class TestsRunner {

public:
	TestsRunner();
	void run();


};

#endif /* _TESTS_RUNNER_H_ */