#include "../../Gpio.h"
#include "../../HeatFlowActuator.h"
#include <unistd.h>
#include <iostream>
using namespace std;

int main() {
	int cnt = 0;

	HeatFlowActuator hfa;
	hfa.init();
	cout << "hfa.init()" << endl;

	do {
		sleep( 3 );
		hfa.on();
		cout << "hfa.on()" << endl;
		sleep( 3 );
		hfa.off();
		cout << "hfa.off()" << endl;
	} while( ++cnt < 3 );

	hfa.deinit();
	cout << "hfa.deinit()" << endl;

	return 0;
}

// eof

