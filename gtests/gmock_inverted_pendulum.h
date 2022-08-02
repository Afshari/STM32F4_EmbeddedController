#ifndef _GMOCK_INVERTED_PENDULUM_H_
#define _GMOCK_INVERTED_PENDULUM_H_

#include <tuple>
#include <iostream>

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <gmock/gmock-matchers.h>

#include "../Inc/Classes/inverted_pendulum.h"
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


class MockInvertedPendulum : public InvertedPendulum {

public:

    MOCK_METHOD1(initialize,       	void(const Matrix &wr));
	MOCK_METHOD1(u,					double(Matrix y));
	MOCK_METHOD1(pendcart,			Matrix(Matrix y));
	MOCK_METHOD3(rungeKutta4,		void(const Matrix &y0, int n, double h));
    MOCK_METHOD0(getDataSize,       int());
	MOCK_METHOD1(getData,			pair<double, double>(int idx));	
};

TEST(InvertedPendulum, Code210) {
	
	string data = "210:0,0,3.1416,0:0,0,3.911592653589793,0:10000,0.001";
	
    shared_ptr<MockInvertedPendulum> inverted_pendulum = make_shared<MockInvertedPendulum>();
	shared_ptr<RobustSuspension> robust_suspension = make_shared<RobustSuspension>();
    shared_ptr<MockInputParser> parser = make_shared<MockInputParser>();

    shared_ptr<vector<int>> indices_1 = make_shared<vector<int>>( vector<int>( { 0, 4, 16, 38 } ) );
	shared_ptr<vector<int>> indices_2 = make_shared<vector<int>>( vector<int>( { 0, 6 } ) );
    Matrix wr { 4, 1, { 0, 0, 0, 0 } };
	Matrix y0 { 4, 1, { 0, 0, 0, 0 } };

	EXPECT_CALL(*parser, getCode(_))
            .Times(1)
            .WillOnce(Return(210));

    EXPECT_CALL(*parser, getIndices(_, _))
            .Times(2)
            .WillOnce(Return(indices_1))
			.WillOnce(Return(indices_2));
			
	EXPECT_CALL(*parser, getVector4d(_))
            .Times(2)
            .WillOnce(Return(wr))
			.WillOnce(Return(y0));

	EXPECT_CALL(*inverted_pendulum, initialize(_))
			.Times(1);
			
	EXPECT_CALL(*inverted_pendulum, getDataSize())
			.Times(1)
			.WillOnce(Return(41));
	
	EXPECT_CALL(*inverted_pendulum, rungeKutta4(_, _, _))
			.Times(1);
			
	EXPECT_CALL(*inverted_pendulum, getData(_))
			.Times(40);

	AppHandler app_handler;
	app_handler.initialize(parser, robust_suspension, inverted_pendulum);
	string result = app_handler.processData(data);
}


TEST(InvertedPendulum, Code211) {
	
	string data = "211:";
	
    shared_ptr<MockInvertedPendulum> inverted_pendulum = make_shared<MockInvertedPendulum>();
	shared_ptr<RobustSuspension> robust_suspension = make_shared<RobustSuspension>();
    shared_ptr<MockInputParser> parser = make_shared<MockInputParser>();

    shared_ptr<vector<int>> indices_1 = make_shared<vector<int>>( vector<int>( { 0, 4 } ) );

	EXPECT_CALL(*parser, getCode(_))
            .Times(1)
            .WillOnce(Return(211));

    EXPECT_CALL(*parser, getIndices(_, _))
            .Times(1)
            .WillOnce(Return(indices_1));
			
	EXPECT_CALL(*parser, getVector4d(_))
            .Times(0);

	EXPECT_CALL(*inverted_pendulum, initialize(_))
			.Times(0);
			
	EXPECT_CALL(*inverted_pendulum, getDataSize())
			.Times(1)
			.WillOnce(Return(41));
	
	EXPECT_CALL(*inverted_pendulum, rungeKutta4(_, _, _))
			.Times(0);
			
	EXPECT_CALL(*inverted_pendulum, getData(_))
			.Times(40);

	AppHandler app_handler;
	app_handler.initialize(parser, robust_suspension, inverted_pendulum);
	string result = app_handler.processData(data);
}

#endif // _GMOCK_INVERTED_PENDULUM_H_
