
#include "inverted_pendulum.h"


InvertedPendulum::InvertedPendulum() {


}

void InvertedPendulum::initialize(const vector<double> &wr) {


	double t_K[] = { -31.6228,  -61.4401,  647.0460,  250.0313 };
	this->K.resize( 1 * 4 );
	std::copy(std::begin(t_K), std::end(t_K), std::begin(this->K));

	this->wr.resize( wr.size() );
	std::copy(std::begin(wr), std::end(wr), std::begin(this->wr));
}

int InvertedPendulum::getDataSize() {

	return values->size();
}

pair<double, double> InvertedPendulum::getData(int idx) {

	return values->at(idx);
}

double InvertedPendulum::u(const vector<double> &y) {

	vector<double> y_wr( 4 * 1 );
	vector<double> res( 1 * 1 );

	sub(y, this->wr, y_wr, 4, 1);
	mul(K, y_wr, res, 1, 4, 1);

	return (-1) * res.at(0);
}

shared_ptr<vector<double>> InvertedPendulum::pendcart(const vector<double> &y) {

		double u = this->u(y);

    double Sx = sin(y.at(2));
    double Cx = cos(y.at(2));
    double D = this->m * this->L * this->L * (this->M + this->m * ( 1 - pow(Cx, 2) ));

    shared_ptr<vector<double>> dx = make_shared<vector<double>>(4);
		dx->at(0) = y[1];
		dx->at(1) = (1/D)*(-pow(this->m, 2) * pow(this->L, 2) * this->g * Cx * Sx + this->m * pow(this->L, 2) * (this->m * this->L * pow(y[3], 2) * Sx - this->d * y[1])) + this->m * this->L * this->L * (1/D) * u;
		dx->at(2) = y[3];
		dx->at(3) = (1/D) * ((this->m + this->M) * this->m * this->g * this->L * Sx - this->m * this->L * Cx *(this->m * this->L * pow(y[3], 2) * Sx - this->d * y[1])) - this->m * this->L * Cx * (1/D) * u;

    return dx;
}


void InvertedPendulum::rungeKutta4(const vector<double> &y0, int n, double h) {

		values = make_unique<vector<pair<double, double>>>( n / STEP_SIZE );
		printf("vector size: %d\r\n", values->size() );
		int values_idx = 0;

		vector<double> y(4 * 1);
		std::copy(std::begin(y0), std::end(y0), std::begin(y));

		vector<double> k1;
		vector<double> y_k1(4 * 1);

		vector<double> k2;
		vector<double> y_k2(4 * 1);

		vector<double> k3;
		vector<double> y_k3(4 * 1);

		vector<double> k4;
		vector<double> sum_k(4 * 1);

    for(int i = 0; i < n; i++) {

    	// K1
    	// k1  = pendcart(y)
    	k1 = *pendcart(y);

    	// K2
    	// k2 = pendcart(y + k1 * h / 2.0);
    	std::copy(std::begin(k1), std::end(k1), std::begin(y_k1));
    	scale(y_k1, h / 2.0, 4, 1);
    	add(y, y_k1, y_k1, 4, 1);
    	k2 = *pendcart(y_k1);


    	// K3
    	// k3 = pendcart(y + k2 * h / 2.0);
    	std::copy(std::begin(k2), std::end(k2), std::begin(y_k2));
    	scale(y_k2, h / 2.0, 4, 1);
			add(y, y_k2, y_k2, 4, 1);
			k3 = *pendcart(y_k2);


			// K4
			// k4 = pendcart(y + k3 * h);
			std::copy(std::begin(k3), std::end(k3), std::begin(y_k3));
			scale(y_k3, h, 4, 1);
			add(y, y_k3, y_k3, 4, 1);
			k4 = *pendcart(y_k3);


			// y->at(i+1) = y->at(i) + (h / 6.) * (k1 + 2*k2 + 2*k3 + k4);
			scale(k2, 2, 4, 1);
			scale(k3, 2, 4, 1);

			add(k1,    k2, sum_k, 4, 1);
			add(sum_k, k3, sum_k, 4, 1);
			add(sum_k, k4, sum_k, 4, 1);
			scale(sum_k, h/6.0, 4, 1);

			add(y, sum_k, y, 4, 1);


			if(i % STEP_SIZE == 0) {
				pair<double, double> val(y[0], y[2]);
				values->at(values_idx) = val;
				values_idx += 1;
			}
    }
}









