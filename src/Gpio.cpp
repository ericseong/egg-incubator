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

	if ( wiringPiSetup() ) {
		cerr << "wiringPiSetup() failed." << endl;
		return;
	}

	_initialized = true;
	return;
}

void Gpio::deinit() {
	_initialized = false;
	return;
}

// EOF

