#ifndef _GMOCK_ROBUST_SUSPENSION_H_
#define _GMOCK_ROBUST_SUSPENSION_H_

#include <tuple>
#include <iostream>

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <gmock/gmock-matchers.h>

#include "../Inc/Classes/robust_suspension.h"
#include "../Inc/Classes/input_parser.h"
#include "../Inc/Classes/app_handler.h"
#include "gmock_input_parser.h"

using std::unique_ptr;
using std::tuple;
using ::testing::_;
using testing::Eq;
using testing::Return;
using testing::ByMove;
using testing::AtLeast;

class MockRobustSuspension : public RobustSuspension {

public:

    MOCK_METHOD0(initialize,       	void());
	MOCK_METHOD2(active,			Matrix(double ms, double w));
};


TEST(RobustSuspension, Code200) {
	
	string data = "200:0.0000,572.2,0.0771,572.2,0.1541,572.2,0.2308,572.2,0.3072,572.2,0.3831";
	
	shared_ptr<InvertedPendulum> inverted_pendulum = make_shared<InvertedPendulum>();
	shared_ptr<MockRobustSuspension> robust_suspension = make_shared<MockRobustSuspension>();
    shared_ptr<MockInputParser> parser = make_shared<MockInputParser>();
	
	shared_ptr<vector<int>> indices_1 = make_shared<vector<int>>( vector<int>( { 0, 4 } ) );
	shared_ptr<vector<int>> indices_2 = make_shared<vector<int>>( vector<int>( { 0,7, 13, 20, 26, 33 } ) );
	Matrix params { 4, 1, { 0, 0, 0, 0 } };
	Matrix mat { 2, 1, { 0, 0 } };
	
	EXPECT_CALL(*parser, getCode(_))
            .Times(1)
            .WillOnce(Return(200));

    EXPECT_CALL(*parser, getIndices(_, _))
            .Times(2)
            .WillOnce(Return(indices_1))
			.WillOnce(Return(indices_2));
			
	EXPECT_CALL(*parser, getDataVector(_, _, _))
            .Times(1)
            .WillOnce(Return(params));
			
	EXPECT_CALL(*robust_suspension, initialize())
			.Times(1);
			
	EXPECT_CALL(*robust_suspension, active(_, _))
			.Times(3)
			.WillRepeatedly(Return(mat));
			
	AppHandler app_handler;
	app_handler.initialize(parser, robust_suspension, inverted_pendulum);
	string result = app_handler.processData(data);
}

TEST(RobustSuspension, Code201) {
	
	string data = "201:0.0000,572.2,0.0771,572.2,0.1541,572.2,0.2308,572.2,0.3072,572.2,0.3831";
	
	shared_ptr<InvertedPendulum> inverted_pendulum = make_shared<InvertedPendulum>();
	shared_ptr<MockRobustSuspension> robust_suspension = make_shared<MockRobustSuspension>();
    shared_ptr<MockInputParser> parser = make_shared<MockInputParser>();
	
	shared_ptr<vector<int>> indices_1 = make_shared<vector<int>>( vector<int>( { 0, 4 } ) );
	shared_ptr<vector<int>> indices_2 = make_shared<vector<int>>( vector<int>( { 0,7, 13, 20, 26, 33 } ) );
	Matrix params { 4, 1, { 0, 0, 0, 0 } };
	Matrix mat { 2, 1, { 0, 0 } };
	
	EXPECT_CALL(*parser, getCode(_))
            .Times(1)
            .WillOnce(Return(201));

    EXPECT_CALL(*parser, getIndices(_, _))
            .Times(2)
            .WillOnce(Return(indices_1))
			.WillOnce(Return(indices_2));
			
	EXPECT_CALL(*parser, getDataVector(_, _, _))
            .Times(1)
            .WillOnce(Return(params));
			
	EXPECT_CALL(*robust_suspension, initialize())
			.Times(0);
			
	EXPECT_CALL(*robust_suspension, active(_, _))
			.Times(3)
			.WillRepeatedly(Return(mat));
			
	AppHandler app_handler;
	app_handler.initialize(parser, robust_suspension, inverted_pendulum);
	string result = app_handler.processData(data);
}

#endif // _GMOCK_ROBUST_SUSPENSION_H_
