// Gpio.cpp
//
// How to build: g++ -lwiringPi
//
// How to use: 
// Gpio& gpio = Gpio::getInstance();
// gpio.init();

#include <iostream>
#include "Gpio.h"

void Gpio::init() {
	if( initialized )
		return;

	if ( wiringPiSetup() ) {
		cerr << "wiringPiSetup() failed." << endl;
	} else {
		initialized = true;
	}

	return;
}

// EOF

