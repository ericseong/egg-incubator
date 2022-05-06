#include <iostream>
#include <unistd.h>
#include "../../TempAlgo.h"

int main() {

	float temp = 22.0;

	TempAlgo ta;
	std::pair<float,float> tt = { 25.1, 25.3 };

	while(1) {
		time_t now;
		time( &now );

		ta.tryGoOnRec( now, temp ); // !!! 

		// simulated temperature
		if( temp < tt.second )
			temp += 0.02;
		else
			temp = tt.second;
		
		tt = ta.getPair( 37.5, 37.7 ); // !!!
		std::cout << "tick: " << now << ", new target: " << "( " << tt.first << ", " << tt.second << " )" << std::endl;
		ta.printRecord();
		sleep(5);
		//sleep(10); // debug
		//sleep(1); // debug
	}

	return  0;
}

// eof

