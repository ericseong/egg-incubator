// HeatActuator.cpp
//

#include <wiringPi.h>
#include "Gpio.h"
#include "HeatActuator.h"

using namespace std;

void HeatActuator::init() {
	if( _initialized )
		return;

	Gpio& gpio = Gpio::getInstance();
	gpio.init();

	pinMode( RELAY_BULB_PIN, OUTPUT );
	_initialized = true;
	return;
}

void HeatActuator::start() {
	if( !_initialized )
		return;
	
	digitalWrite( RELAY_BULB_PIN, HIGH );
	_level = ON;
	delay(1);
	return;
}	

void HeatActuator::stop() {
	if( !_initialized )
		return;

	digitalWrite( RELAY_BULB_PIN, LOW );
	_level = OFF;
	delay(1);
	return;
}

level_t HeatActuator::get() {
	if( !_initialized ) 
		return INVALID

	return _level;
} 

// EOF

