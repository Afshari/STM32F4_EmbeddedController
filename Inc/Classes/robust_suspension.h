

#ifndef _ROBUST_SUSPENSION_H_
#define _ROBUST_SUSPENSION_H_

#include <vector>
#include <memory>

#include "matrix.h"

using std::unique_ptr;
using std::make_unique;
using std::shared_ptr;
using std::make_shared;
using std::vector;

class RobustSuspension {
public:
	
		RobustSuspension();
	
		virtual void initialize();
    virtual Matrix active(double ms, double w);

protected:

    const int nu = 1;
    const int nx = 4;
    const int nz = 3;

    const double dt = 0.001;

    const double   a      = 0.1;           // height of the bump
    const double   l      = 2;             // length of the bump
    const double   v0     = 45/3.6;        // vehicle forward velocity (m/s)
    const int      nb     = 0;             // maximum value of time with nonzero ground variation

    const double   x1r = 0.01;               // constant reference suspension deflection
    //vector<double> ur = { 42720 * x1r };
    //vector<double> u  = { 0 };
		Matrix ur { 1, 1, { 42720 * x1r } };
		Matrix u  { 1, 1, { 0 } };

		//vector<double>  xr;
		//vector<double>  xp;
		//vector<double>  x;
		//vector<double> 	D;
		//vector<double> 	K;
		Matrix xr { 1, 1 };
		Matrix xp { 1, 1 };
		Matrix x  { 1, 1 };
		Matrix D  { 1, 1 };
		Matrix K  { 1, 1 };


    const double mu =   113.6;                   // unsprung mass
    const double ks =   42719.6;                 // stiffness of passive suspension
    const double cs =   1095;                    // damping of passive suspension
    const double kt =   101115;                  // stiffness of pneumatic tyre
    const double ct =   14.6;                    // damping of pneumatic tyre

    const int alpha  =   21;                     // positive weighting for suspension deflection
    const int beta   =   42;                     // positive weighting for tyre deflection


    friend class TestRobustSuspension;

};

#endif /* _ROBUST_SUSPENSION_H_ */
