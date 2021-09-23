#include "../../Gpio.h"
#include "../../Dht22TempSensor.h"
#include <unistd.h>
#include <iostream>
using namespace std;

int main() {
	float val;

	Dht22TempSensor ts;
	ts.init();
	cout << "ts.init()" << endl;


	while(1) {
		if( ts.get( val ) != -1 ) {
			cout << "temp: " << val << endl; 
		}
		sleep(3);
	}

	ts.deinit();
	cout << "ts.deinit()" << endl;

	return 0;
}

// eof

