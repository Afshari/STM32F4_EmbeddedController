
#include "inverted_pendulum.h"


InvertedPendulum::InvertedPendulum() {


}

void InvertedPendulum::initialize(const Matrix &wr) {

	K = Matrix{ 1, 4, { -31.6228,  -61.4401,  647.0460,  250.0313 } };
	this->wr = wr;
}

int InvertedPendulum::getDataSize() {

	return values->size();
}

pair<double, double> InvertedPendulum::getData(int idx) {

	return values->at(idx);
}

double InvertedPendulum::u(Matrix y) {

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
		int values_idx = 0;

		Matrix y = y0;
		Matrix k1{ 4, 1 };
		Matrix k2{ 4, 1 };
		Matrix k3{ 4, 1 };
		Matrix k4{ 4, 1 };

    for(int i = 0; i < n; i++) {

    	k1 = pendcart(y);
			k2 = pendcart(y + k1 * (h / 2.0));
			k3 = pendcart(y + k2 * (h / 2.0));
			k4 = pendcart(y + k3 * h);
			
			y = y + (h / 6.0) * (k1 + 2*k2 + 2*k3 + k4);

			if(i % STEP_SIZE == 0) {
				pair<double, double> val(y.at(0, 0), y.at(2, 0));
				values->at(values_idx) = val;
				values_idx += 1;
			}
    }
}









