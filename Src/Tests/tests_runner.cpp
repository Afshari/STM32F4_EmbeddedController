
#include "Tests/tests_runner.h"


TestsRunner::TestsRunner() {

}


void TestsRunner::run() {

	TestInputParser test_input_parser;
	test_input_parser.init();

	TestNetworkDataHandler test_data_network_handler;
	test_data_network_handler.init();

	TestLinearAlgebra test_linear_algebra;
	test_linear_algebra.init();

	TestInvertedPendulum test_inverted_pendulum;
	test_inverted_pendulum.init();
	
	TestRobustSuspension test_robust_suspension;
	test_robust_suspension.init();
	
	while(1);

}
