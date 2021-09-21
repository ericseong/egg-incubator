#include "../../Gpio.h"
#include "../../HumidActuator.h"
#include <unistd.h>
#include <iostream>
using namespace std;

int main() {
	int cnt = 0;

	HumidActuator ha;
	ha.init();
	cout << "ha.init()" << endl;

	while( ++cnt <= 10 ) {
		ha.start( LEVEL_50 );
		cout << "ha.start(LEVEL_50)" << endl;
		sleep(3);
		ha.stop();
		cout << "ha.stop()" << endl;
		sleep(3);
	}

	ha.on();
	cout << "ha.on()" << endl;
	sleep(10);

	ha.deinit();
	cout << "ha.deinit()" << endl;

	return 0;
}

// eof

