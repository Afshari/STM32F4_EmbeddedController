
#include "app_handler.h"

AppHandler::AppHandler() {

	initialize();
}


void AppHandler::initialize() {

	parser = make_unique<InputParser>();
	robust_suspension = make_unique<RobustSuspension>();
	robust_suspension->initialize();
	inverted_pendulum = make_unique<InvertedPendulum>();
}

string AppHandler::processData(const string &data) {

	string result_value = "";

	int  code = parser->getCode(data);
	auto indices = parser->getIndices(data, ":");

	int start_index = indices->at(1);
	int len = data.length() - indices->at(1);

	if(code == 101) {
		robust_suspension->initialize();
	}

	if(code == 101 || code == 102) {

		shared_ptr<vector<float>> params = parser->getDataVector(data, start_index, len);
		string curr_data = data.substr(start_index, len);

		indices = parser->getIndices( curr_data, "," );

		for(uint16_t i = 0; i < indices->size() - 1; i += 2) {

				string strW = curr_data.substr( indices->at(i), indices->at(i+1) - indices->at(i) - 1 );
				string strMs = "";
				if(i == indices->size() - 2)
						strMs = curr_data.substr( indices->at(i+1), curr_data.length() - indices->at(i+1) );
				else
						strMs = curr_data.substr( indices->at(i+1), indices->at(i+2) - indices->at(i+1) - 1 );

				double w    = std::atof(strW.c_str());
				double ms   = std::atof(strMs.c_str());

				shared_ptr<vector<double>> x = robust_suspension->active(ms, w);
				// double v1 = x->at(0);
				// double v2 = x->at(1);

				if(result_value != "")
						result_value += ",";
				result_value += std::to_string(x->at(0)) + "," + std::to_string(x->at(1));
		}
	}

	if(code == 111) {
		
		vector<double> wr = *parser->getVector4d( data.substr( indices->at(1), indices->at(2) - indices->at(1) - 1 ) );
		vector<double> y0 = *parser->getVector4d( data.substr( indices->at(2), indices->at(3) - indices->at(2) - 1 ) );

		string init_data = data.substr( indices->at(3), data.length() - indices->at(3) );
		indices = parser->getIndices(init_data, ",");
		int n = std::atoi( init_data.substr( indices->at(0),    indices->at(1) - indices->at(0) - 1 ).c_str() );
		double h = std::atof( init_data.substr( indices->at(1), init_data.length() - indices->at(1) ).c_str() );

		inverted_pendulum->initialize(wr);
		inverted_pendulum->rungeKutta4(y0, n, h);

		pendulum_counter = 0;
	}

	if(code == 111 || code == 112) {

			if( (pendulum_counter+1) * InvertedPendulum::ITEM_PER_STEP <= inverted_pendulum->getDataSize() ) {

					for(int i = pendulum_counter * InvertedPendulum::ITEM_PER_STEP; i < (pendulum_counter+1) * InvertedPendulum::ITEM_PER_STEP; i++) {

							if(result_value != "")
								result_value += ";";
							pair<double, double> y = inverted_pendulum->getData(i);
							result_value += std::to_string(y.first) + "," + std::to_string(y.second);
					}
					pendulum_counter += 1;
			} else {
				result_value = "finished";
			}
	}

	return result_value;
}










