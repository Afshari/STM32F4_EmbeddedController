
#include "Tests/test_robust_suspension.h"

TestRobustSuspension::TestRobustSuspension() {


}

void TestRobustSuspension::init() {

	testInitialize();
	testActive_1();
	testActive_2();

	printReport("Robust Suspension");
}


void TestRobustSuspension::testInitialize() {


	bool status = false;
	float matrixTolerance = 0.01;

	RobustSuspension robust_suspension;
	robust_suspension.initialize();

	//vector<double> D = { 0, -1, 0, 0.12852 };
	Matrix D { 4, 1, { 0, -1, 0, 0.12852 } };
	status = checkEqual(robust_suspension.D, D, matrixTolerance, "D Vector value is not Correct");

	//vector<double> xp = { 0, 0, 0, 0 };
	Matrix xp { 4, 1, { 0, 0, 0, 0 } };
	status = checkEqual(robust_suspension.xp, xp, matrixTolerance, "xp Vector value is not Correct");

	//vector<double> x = { 0, 0, 0, 0 };
	Matrix x { 4, 1, { 0, 0, 0, 0 } };
	status = checkEqual(robust_suspension.x, x, matrixTolerance, "x Vector value is not Correct");

	//vector<double> xr = { 0.01, -3.95e-8, 0, 0 };
	Matrix xr { 4, 1, { 0.01, -3.95e-8, 0, 0 } };
	status = checkEqual(robust_suspension.xr, xr, matrixTolerance, "xr Vector value is not Correct");

	//vector<double> K = { -90156, 23219, -34989, 1148.4 };
	Matrix K { 1, 4, { -90156, 23219, -34989, 1148.4 } };
	status = checkEqual(robust_suspension.K, K, matrixTolerance, "K Matrix value is not Correct");
}


void TestRobustSuspension::testActive_1() {

	double ref_x[4 * 1] = { -0.08955920740740497, 0.08725703488722845, 0.42492189363303334, -0.3696008286356674 };

	double ms = 572.2;
	double w = -1.6741550478065024;

	double x[4 * 1] = { -0.09028486506606254, 0.08584334904761125, 0.42620159445832106, -0.2994560641992414 };

	RobustSuspension robust_suspension;
	robust_suspension.initialize();
	//std::copy(std::begin(x), std::end(x), std::begin(robust_suspension.x));
	robust_suspension.x = Matrix{ 4, 1, { -0.09028486506606254, 0.08584334904761125, 0.42620159445832106, -0.2994560641992414 } };
	Matrix result_x = robust_suspension.active(ms, w);

	for(unsigned int i = 0; i < result_x.rowSize(); i++)
		checkEqual(result_x.at(i, 0), ref_x[i], 1e-4, " ");
}

void TestRobustSuspension::testActive_2() {

	double ref_x_1[4 * 1] = { -0.09027282496542552, 0.0394048871722823, 0.39126809198309526, 1.0202228949225869 };

  double ms = 572.2;
  double w = -1.8897764376573805;

  //double x[4 * 1] = { -0.08958431728394818, 0.036462142328820786, 0.3868420590207073, 1.0753497404980417 };

	RobustSuspension robust_suspension;
	robust_suspension.initialize();
	//std::copy(std::begin(x), std::end(x), std::begin(robust_suspension.x));
	robust_suspension.x = Matrix{ 4, 1, { -0.08958431728394818, 0.036462142328820786, 0.3868420590207073, 1.0753497404980417 } };
	Matrix result_x = robust_suspension.active(ms, w);

	for(unsigned int i = 0; i < result_x.rowSize(); i++)
		checkEqual(result_x.at(i, 0), ref_x_1[i], 1e-4, " ");


	double ref_x_2[4 * 1] = { -0.09090177976836501, 0.042314886504862266, 0.39547817409791675, 0.9635686683135917 };
	w = -1.9092438740581472;
	result_x = robust_suspension.active(ms, w);

	for(unsigned int i = 0; i < result_x.rowSize(); i++)
		checkEqual(result_x.at(i, 0), ref_x_2[i], 1e-4, " ");


}













