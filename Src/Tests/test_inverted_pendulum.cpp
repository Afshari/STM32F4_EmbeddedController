
#include "Tests/test_inverted_pendulum.h"

TestInvertedPendulum::TestInvertedPendulum() {
}


void TestInvertedPendulum::init() {

	testInitialize();
	testU();
	testPendcart();
	testRungeKutta4();

	printReport("Inverted Pendulum");
}

void TestInvertedPendulum::testInitialize() {

	bool status = false;
	float matrixTolerance = 0.01;

	//vector<double> wr( { 1, 0, M_PI, 0 } );
	Matrix wr { 4, 1, { 1, 0, M_PI, 0 } };

	InvertedPendulum inverted_pendulum;
	inverted_pendulum.initialize(wr);

	//vector<double> K( { -31.6228,  -61.4401,  647.0460,  250.0313 } );
	Matrix K { 1, 4, { -31.6228,  -61.4401,  647.0460,  250.0313 } };
	status = checkEqual(inverted_pendulum.K,  K, matrixTolerance, "K value is not Correct");
	status = checkEqual(inverted_pendulum.wr, wr, matrixTolerance, "wr value is not Correct");
}


void TestInvertedPendulum::testU() {

	//vector<double> wr( { 1, 0, M_PI, 0 } );
	Matrix wr { 4, 1, { 1, 0, M_PI, 0 } };

	//vector<double> y( { -1.0000, 0, 3.7416, 0 } );
	Matrix y { 4, 1, { -1.0000, 0, 3.7416, 0 } };

	InvertedPendulum inverted_pendulum;
	inverted_pendulum.initialize(wr);

	double res = inverted_pendulum.u(y);

	checkEqual(res, -451.4779, 1e-2, "Result of u function is not Correct");
}


void TestInvertedPendulum::testPendcart() {


	double ref_y[] = { 0., -84.00590891, 0. , -31.84311733 };

	//vector<double> y0( { -1.0000, 0, 3.7416, 0 } );
	Matrix y0 { 4, 1, { -1.0000, 0, 3.7416, 0 } };
	//vector<double> wr( { 1, 0, M_PI, 0 } );
	Matrix wr { 4, 1, { 1, 0, M_PI, 0 } };
	InvertedPendulum inverted_pendulum;
	inverted_pendulum.initialize(wr);

	Matrix y = inverted_pendulum.pendcart(y0);
	for(uint8_t i = 0; i < 4; i++)
		checkEqual(y.at(i, 0), ref_y[i], 1e-2, "Result of Pendcart function is not Correct");
}



void TestInvertedPendulum::testRungeKutta4() {

	int n = 10000;
	double h = 0.001;

	//vector<double> y0( { -1.0000, 0, 3.7416, 0 } );
	Matrix y0 { 4, 1, { -1.0000, 0, 3.7416, 0 } };
	//vector<double> wr( { 1, 0, M_PI, 0 } );
	Matrix wr { 4, 1, { 1, 0, M_PI, 0 } };
	InvertedPendulum inverted_pendulum;
	inverted_pendulum.initialize(wr);

	inverted_pendulum.rungeKutta4(y0, n, h);

	double v1 = inverted_pendulum.values->at(249).first;
	double v2 = inverted_pendulum.values->at(249).second;

	checkEqual(inverted_pendulum.values->size(), n/inverted_pendulum.STEP_SIZE, "Result of RungeKutta function is not Correct");


}








