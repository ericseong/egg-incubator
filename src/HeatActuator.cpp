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

	pinMode( RELAY_BULB1_PIN, OUTPUT );
	//pinMode( RELAY_BULB2_PIN, OUTPUT );

	_initialized = true;
	off();

	return;
}

void HeatActuator::deinit() {
	if( !_initialized )
		return;
	
	off();

	Gpio& gpio = Gpio::getInstance();
	gpio.deinit();
	_initialized = false;
	return;
}

void HeatActuator::on() {
	if( !_initialized )
		return;
	
	digitalWrite( RELAY_BULB1_PIN, HIGH );
	//digitalWrite( RELAY_BULB2_PIN, HIGH );
	_level = LEVEL_ON;

	delay(1);
	return;
}	

void HeatActuator::start( level_t level ) {
	on();
	return;
}

void HeatActuator::off() {
	if( !_initialized )
		return;

	digitalWrite( RELAY_BULB1_PIN, LOW );
	//digitalWrite( RELAY_BULB2_PIN, LOW );
	_level = LEVEL_OFF;

	delay(1);
	return;
}

void HeatActuator::stop() {
	off();
	return;
}

level_t HeatActuator::get() {
	if( !_initialized ) 
		return LEVEL_INVALID;

	return _level;
} 

// EOF

