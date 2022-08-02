#ifndef _INVERTED_PENDULUM_H_
#define _INVERTED_PENDULUM_H_

#include <vector>
#include <memory>
#include <cmath>

#include "matrix.h"

using std::unique_ptr;
using std::make_unique;
using std::shared_ptr;
using std::make_shared;
using std::vector;
using std::pair;

class InvertedPendulum {
	
public:
	InvertedPendulum();

	void initialize(const Matrix &wr);
	double u(Matrix y);
	Matrix pendcart(Matrix y);
	void rungeKutta4(const Matrix &y0, int n, double h);

	int getDataSize();
	pair<double, double> getData(int idx);

	static const int STEP_SIZE 	= 20;
	static const int ITEM_PER_STEP = 40;

protected:


	unique_ptr<vector<pair<double, double>>> values;

    double m   =   1;
    double M   =   5;
    double L   =   2;
    double g   =   -10;
    double d   =   1;

    //vector<double> K;
    //vector<double> wr;
		Matrix K  { 1, 1 };
		Matrix wr { 1, 1 };

    friend class TestInvertedPendulum;

};

#endif /* _INVERTED_PENDULUM_H_ */