// RollerActuator.cpp
//

#include <wiringPi.h>
#include "Gpio.h"
#include "RollerActuator.h"

using namespace std;

void RollerActuator::init() {
	if( _initialized )
		return;

	Gpio& gpio = Gpio::getInstance();
	gpio.init();

	pinMode( RELAY_MOTOR_PIN, OUTPUT );
	_initialized = true;
	return;
}

void RollerActuator::start() {
	if( !_initialized )
		return;
	
	digitalWrite( RELAY_MOTOR_PIN, HIGH );
	_level = LEVEL_ON;
	delay(1);
	return;
}	

void RollerActuator::stop() {
	if( !_initialized )
		return;

	digitalWrite( RELAY_MOTOR_PIN, LOW );
	_level = LEVEL_OFF;
	delay(1);
	return;
}

level_t RollerActuator::get() {
	if( !_initialized ) 
		return LEVEL_INVALID;

	return _level;
} 

// EOF

