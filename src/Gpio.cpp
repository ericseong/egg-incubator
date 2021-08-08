// Gpio.cpp
//
// How to build: g++ -lwiringPi
//
// How to use: 
// Gpio& gpio = Gpio::getInstance();
// gpio.init();

#include <iostream>
#include <wiringPi.h>
#include "Gpio.h"

using namespace std;

void Gpio::init() {
	if( _initialized )
		return;

#ifndef SKIP_WIRINGPI_SETUP
	if ( wiringPiSetup() ) {
		cerr << "wiringPiSetup() failed." << endl;
		return;
	}
#endif

	_initialized = true;
	return;
}

void Gpio::deinit() {
	_initialized = false;
	return;
}

// EOF

