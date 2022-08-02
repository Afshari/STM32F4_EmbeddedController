
#include "app_handler.h"

AppHandler::AppHandler() {

}


void AppHandler::initialize(shared_ptr<InputParser> parser, shared_ptr<RobustSuspension> robust_suspension, 
		shared_ptr<InvertedPendulum> inverted_pendulum) {

	this->parser = parser;
	this->robust_suspension = robust_suspension;
	this->inverted_pendulum = inverted_pendulum;
}

string AppHandler::processData(const string &data) {

	string result_value = "";

	int  code = parser->getCode(data);
	auto indices = parser->getIndices(data, ":");

	int start_index = indices->at(1);
	int len = data.length() - indices->at(1);

	if(code == 200) {
		robust_suspension->initialize();
	}

	if(code == 200 || code == 201) {

		Matrix params = parser->getDataVector(data, start_index, len);
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

				Matrix x = robust_suspension->active(ms, w);

				if(result_value != "")
						result_value += ",";
				result_value += std::to_string(x.at(0, 0)) + "," + std::to_string(x.at(1, 0));
		}
	}

	if(code == 210) {
		
		Matrix wr = parser->getVector4d(data.substr(indices->at(1), indices->at(2) - indices->at(1) - 1));
		Matrix y0 = parser->getVector4d(data.substr(indices->at(2), indices->at(3) - indices->at(2) - 1));

		string init_data = data.substr(indices->at(3), data.length() - indices->at(3));
		indices = parser->getIndices(init_data, ",");
		int n = std::atoi( init_data.substr( indices->at(0), indices->at(1) - indices->at(0) - 1 ).c_str() );
		double h = std::atof( init_data.substr( indices->at(1), init_data.length() - indices->at(1) ).c_str() );

		inverted_pendulum->initialize(wr);
		inverted_pendulum->rungeKutta4(y0, n, h);

		pendulum_counter = 0;
	}

	if(code == 210 || code == 211) {

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










