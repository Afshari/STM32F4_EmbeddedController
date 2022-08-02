
#include "inverted_pendulum.h"


InvertedPendulum::InvertedPendulum() {


}

void InvertedPendulum::initialize(const Matrix &wr) {


	//double t_K[] = { -31.6228,  -61.4401,  647.0460,  250.0313 };
	//this->K.resize( 1 * 4 );
	//std::copy(std::begin(t_K), std::end(t_K), std::begin(this->K));
	K = Matrix{ 1, 4, { -31.6228,  -61.4401,  647.0460,  250.0313 } };

	//this->wr.resize( wr.size() );
	//std::copy(std::begin(wr), std::end(wr), std::begin(this->wr));
	this->wr = wr;
}

int InvertedPendulum::getDataSize() {

	return values->size();
}

pair<double, double> InvertedPendulum::getData(int idx) {

	return values->at(idx);
}

double InvertedPendulum::u(Matrix y) {

	//vector<double> y_wr( 4 * 1 );
	//vector<double> res( 1 * 1 );

	//sub(y, this->wr, y_wr, 4, 1);
	//mul(K, y_wr, res, 1, 4, 1);
	Matrix res = K * (y - wr);

	return (-1) * res.at(0, 0);
}

Matrix InvertedPendulum::pendcart(Matrix y) {

		double u = this->u(y);

    double Sx = sin(y.at(2, 0));
    double Cx = cos(y.at(2, 0));
    double D = this->m * this->L * this->L * (this->M + this->m * ( 1 - pow(Cx, 2) ));

    Matrix dx { 4, 1, { 0, 0, 0, 0 } };
		dx.set(y.at(1, 0), 0, 0);
		dx.set((1/D)*(-pow(this->m, 2) * pow(this->L, 2) * this->g * Cx * Sx + this->m * pow(this->L, 2) * (this->m * this->L * pow(y.at(3, 0), 2) * Sx - this->d * y.at(1, 0))) + this->m * this->L * this->L * (1/D) * u, 1, 0);
		dx.set(y.at(3, 0), 2, 0);
		dx.set((1/D) * ((this->m + this->M) * this->m * this->g * this->L * Sx - this->m * this->L * Cx *(this->m * this->L * pow(y.at(3, 0), 2) * Sx - this->d * y.at(1, 0))) - this->m * this->L * Cx * (1/D) * u, 3, 0);

    return dx;
}


void InvertedPendulum::rungeKutta4(const Matrix &y0, int n, double h) {

		values = make_unique<vector<pair<double, double>>>( n / STEP_SIZE );
		//printf("vector size: %d\r\n", values->size() );
		int values_idx = 0;

		//vector<double> y(4 * 1);
		//std::copy(std::begin(y0), std::end(y0), std::begin(y));
		Matrix y = y0;

		//vector<double> k1;
		//vector<double> y_k1(4 * 1);
		Matrix k1{ 4, 1 };

		//vector<double> k2;
		//vector<double> y_k2(4 * 1);
		Matrix k2{ 4, 1 };

		//vector<double> k3;
		//vector<double> y_k3(4 * 1);
		Matrix k3{ 4, 1 };

		//vector<double> k4;
		//vector<double> sum_k(4 * 1);
		Matrix k4{ 4, 1 };

    for(int i = 0; i < n; i++) {

    	// K1
    	// k1  = pendcart(y)
    	k1 = pendcart(y);

    	// K2
    	// k2 = pendcart(y + k1 * h / 2.0);
    	//std::copy(std::begin(k1), std::end(k1), std::begin(y_k1));
    	//scale(y_k1, h / 2.0, 4, 1);
    	//add(y, y_k1, y_k1, 4, 1);
    	//k2 = *pendcart(y_k1);
			k2 = pendcart(y + k1 * (h / 2.0));

    	// K3
    	// k3 = pendcart(y + k2 * h / 2.0);
    	//std::copy(std::begin(k2), std::end(k2), std::begin(y_k2));
    	//scale(y_k2, h / 2.0, 4, 1);
			//add(y, y_k2, y_k2, 4, 1);
			//k3 = *pendcart(y_k2);
			k3 = pendcart(y + k2 * (h / 2.0));


			// K4
			// k4 = pendcart(y + k3 * h);
			//std::copy(std::begin(k3), std::end(k3), std::begin(y_k3));
			//scale(y_k3, h, 4, 1);
			//add(y, y_k3, y_k3, 4, 1);
			//k4 = *pendcart(y_k3);
			k4 = pendcart(y + k3 * h);


			// y->at(i+1) = y->at(i) + (h / 6.) * (k1 + 2*k2 + 2*k3 + k4);
			//scale(k2, 2, 4, 1);
			//scale(k3, 2, 4, 1);
			//
			//add(k1,    k2, sum_k, 4, 1);
			//add(sum_k, k3, sum_k, 4, 1);
			//add(sum_k, k4, sum_k, 4, 1);
			//scale(sum_k, h/6.0, 4, 1);
			//
			//add(y, sum_k, y, 4, 1);
			y = y + (h / 6.0) * (k1 + 2*k2 + 2*k3 + k4);


			if(i % STEP_SIZE == 0) {
				//pair<double, double> val(y[0], y[2]);
				pair<double, double> val(y.at(0, 0), y.at(2, 0));
				values->at(values_idx) = val;
				values_idx += 1;
			}
    }
}









