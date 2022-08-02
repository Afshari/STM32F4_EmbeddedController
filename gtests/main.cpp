
#include <iostream>
#include <gtest/gtest.h>

#include "gmock_input_parser.h"
#include "gmock_inverted_pendulum.h"
#include "gmock_robust_suspension.h"


int main(int argc, char *argv[]) {
	
	::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
	return 0;
}