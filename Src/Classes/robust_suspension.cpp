
#include "robust_suspension.h"

RobustSuspension::RobustSuspension() {

}

void RobustSuspension::initialize() {

	//xr.resize(4 * 1);
	//xp.resize(4 * 1);
	//x.resize(4 * 1);
	//D.resize(4 * 1);
	//K.resize(1 * 4);
	
	//double t_xr[] = { 1, -3.9559*1e-6, 0, 0 };
	//std::copy(std::begin(t_xr), std::end(t_xr), std::begin(this->xr));
	this->xr = { 4, 1, { 1, -3.9559*1e-6, 0, 0 } };
	this->xr = x1r * this->xr;
	//scale(this->xr, x1r, 4, 1);

	//double t_xp[] = { 0, 0, 0, 0 };
	//std::copy(std::begin(t_xp), std::end(t_xp), std::begin(this->xp));
	this->xp = { 4, 1, { 0, 0, 0, 0 } };

	//double t_x[] = { 0, 0, 0, 0 };
	//std::copy(std::begin(t_x), std::end(t_x), std::begin(this->x));
	this->x = { 4, 1, { 0, 0, 0, 0 } };

	//double t_D[] = { 0, -1, 0, ct/mu };
	//std::copy(std::begin(t_D), std::end(t_D), std::begin(this->D));
	this->D = { 4, 1, { 0, -1, 0, ct/mu } };

	//double t_K[] = { -90156, 23219, -34989, 1148.4 };
	//std::copy(std::begin(t_K), std::end(t_K), std::begin(this->K));
	this->K = { 1, 4, { -90156, 23219, -34989, 1148.4 } };
}



Matrix RobustSuspension::active(double ms, double _w) {

	//vector<double> w( { _w } );
	Matrix w = { 1, 1, { _w } };

	//vector<double> A( {
	Matrix A = { 4, 4, {
				0,          0,          1,         -1,
				0,          0,          0,          1,
		   -ks/ms,      0,         -cs/ms,      cs/ms,
				ks/mu,     -kt/mu,      cs/mu,     -(cs+ct)/mu } };

	//vector<double> B( { 0, 0, 1/ms, -1/mu } );
	Matrix B = { 4, 1, { 0, 0, 1/ms, -1/mu } };

	//vector<double> C( {
	Matrix C = { 4, 4, {
	   -ks/ms,          				0,         				 -cs/ms,      cs/ms,
			(double)alpha,          0,          				0,          0,
			0,              				(double)beta,       0,          0				} };

	//vector<double> x_xr( 4 * 1 );
	//vector<double> K_x_xr( 1 * 1 );

	// u(i) = ur(i) + K * (x(:,i) - xr(:,i));
	//sub(x, xr, x_xr, 4, 1);
	//mul(K, x_xr, K_x_xr, 1, 4, 1);
	//add(ur, K_x_xr, u, 1, 1);
	this->u = this->ur + K * (x - xr);

	//vector<double> A_x( 4 * 1 );
	//vector<double> B_u( 4 * 1 );
	//vector<double> D_w( 4 * 1 );

	//vector<double> Ax_Bu( 4 * 1 );
	//vector<double> Ax_Bu_Dw( 4 * 1 );

	// x(:,i+1) = x(:,i) + ( A * x(:,i) + B * u(i) + D * w(i) ) * dt ;
	//	*result_x = (*x) + ( (*A) * (*x) + (*B) * u + (*D) * w ) * dt;
	x = x + (A * x + B * u + D * w) * dt;
	//mul(A, x, A_x, 4, 4, 1);
	//mul(B, u, B_u, 4, 1, 1);
	//mul(D, w, D_w, 4, 1, 1);
	//add(A_x, B_u, Ax_Bu, 4, 1);
	//add(Ax_Bu, D_w, Ax_Bu_Dw, 4, 1);
	//scale(Ax_Bu_Dw, dt, 4, 1);
	//add(x, Ax_Bu_Dw, x, 4, 1);

	//shared_ptr<vector<double>> result_x = make_shared<vector<double>>(4);
	//std::copy(std::begin(x), std::end(x), std::begin(*result_x));

	return x;
}




