
#include "linalg.h"


void add( const vector<double> &A, const vector<double> &B, shared_ptr<vector<double>> C, 
		int row, int column ) {
	
	/*
	 * This uses the formula C = A + B
	 */
	C = make_shared<vector<double>>( row * column );

	// Add all values
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < column; j++) {

			int idx = (i * column) + j;
			C->at(idx) = A.at(idx) + B.at(idx);

		}
	}
}

