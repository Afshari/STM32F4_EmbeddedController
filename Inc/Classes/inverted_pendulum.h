#ifndef _INVERTED_PENDULUM_H_
#define _INVERTED_PENDULUM_H_

#include <vector>
#include <memory>
#include <cmath>

#include "linalg.h"

using std::unique_ptr;
using std::make_unique;
using std::shared_ptr;
using std::make_shared;
using std::vector;
using std::pair;

class InvertedPendulum {
	
public:
	InvertedPendulum();

	void initialize(const vector<double> &wr);
	double u(const vector<double> &y);
	shared_ptr<vector<double>> pendcart(const vector<double> &y);
	void rungeKutta4(const vector<double> &y0, int n, double h);

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

    vector<double> K;
    vector<double> wr;

    friend class TestInvertedPendulum;

};

#endif /* _INVERTED_PENDULUM_H_ */