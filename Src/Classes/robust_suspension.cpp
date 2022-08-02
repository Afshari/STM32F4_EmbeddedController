
#include "robust_suspension.h"

RobustSuspension::RobustSuspension() {

}

void RobustSuspension::initialize() {

	this->xr = { 4, 1, { 1, -3.9559*1e-6, 0, 0 } };
	this->xr = x1r * this->xr;
	this->xp = { 4, 1, { 0, 0, 0, 0 } };
	this->x = { 4, 1, { 0, 0, 0, 0 } };
	this->D = { 4, 1, { 0, -1, 0, ct/mu } };
	this->K = { 1, 4, { -90156, 23219, -34989, 1148.4 } };
}



Matrix RobustSuspension::active(double ms, double _w) {

	Matrix w = { 1, 1, { _w } };

	Matrix A = { 4, 4, {
				0,          0,          1,         -1,
				0,          0,          0,          1,
		   -ks/ms,      0,         -cs/ms,      cs/ms,
				ks/mu,     -kt/mu,      cs/mu,     -(cs+ct)/mu } };

	Matrix B = { 4, 1, { 0, 0, 1/ms, -1/mu } };

	Matrix C = { 4, 4, {
	   -ks/ms,          				0,         				 -cs/ms,      cs/ms,
			(double)alpha,          0,          				0,          0,
			0,              				(double)beta,       0,          0				} };

	this->u = this->ur + K * (x - xr);

	x = x + (A * x + B * u + D * w) * dt;

	return x;
}




