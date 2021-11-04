
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

	vector<double> wr( { 1, 0, M_PI, 0 } );

	InvertedPendulum inverted_pendulum;
	inverted_pendulum.initialize(wr);

	vector<double> K( { -31.6228,  -61.4401,  647.0460,  250.0313 } );
	status = checkEqual(inverted_pendulum.K,  K, matrixTolerance, " ");
	status = checkEqual(inverted_pendulum.wr, wr, matrixTolerance, " ");
}


void TestInvertedPendulum::testU() {

	vector<double> wr( { 1, 0, M_PI, 0 } );

	vector<double> y( { -1.0000, 0, 3.7416, 0  } );

	InvertedPendulum inverted_pendulum;
	inverted_pendulum.initialize(wr);

	double res = inverted_pendulum.u(y);

	checkEqual(res, -451.4779, 1e-2, " ");
}


void TestInvertedPendulum::testPendcart() {


	double ref_y[] = { 0., -84.00590891, 0. , -31.84311733 };

	vector<double> y0( { -1.0000, 0, 3.7416, 0 } );
	vector<double> wr( { 1, 0, M_PI, 0 } );
	InvertedPendulum inverted_pendulum;
	inverted_pendulum.initialize(wr);

	vector<double> y = *inverted_pendulum.pendcart(y0);
	for(uint8_t i = 0; i < 4; i++)
		checkEqual(y[i], ref_y[i], 1e-2, " ");
}



void TestInvertedPendulum::testRungeKutta4() {

	int n = 10000;
	double h = 0.001;

	vector<double> y0( { -1.0000, 0, 3.7416, 0 } );
	vector<double> wr( { 1, 0, M_PI, 0 } );
	InvertedPendulum inverted_pendulum;
	inverted_pendulum.initialize(wr);

	inverted_pendulum.rungeKutta4(y0, n, h);

	double v1 = inverted_pendulum.values->at(249).first;
	double v2 = inverted_pendulum.values->at(249).second;

	checkEqual(inverted_pendulum.values->size(), n/inverted_pendulum.STEP_SIZE, " ");


}








