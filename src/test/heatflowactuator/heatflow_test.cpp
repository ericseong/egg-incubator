#include "../../Gpio.h"
#include "../../HeatActuator.h"
#include "../../HeatFlowActuator.h"
#include <unistd.h>
#include <iostream>
using namespace std;

int main() {
	int cnt = 0;

	HeatActuator ha;
	ha.init();
	cout << "ha.init()" << endl;

	HeatFlowActuator hfa;
	hfa.init();
	cout << "hfa.init()" << endl;

	do {
		sleep( 3 );
		hfa.on();
		ha.off();
		cout << "hfa.on()" << endl;
		sleep( 3 );
		hfa.off();
		ha.on();
		cout << "hfa.off()" << endl;
	} while( ++cnt < 3 );

	hfa.deinit();
	cout << "hfa.deinit()" << endl;

	ha.deinit();
	cout << "ha.deinit()" << endl;

	return 0;
}

// eof

