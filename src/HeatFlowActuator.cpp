// HeatFlowActuator.cpp
//

#include <wiringPi.h>
#include "Gpio.h"
#include "HeatFlowActuator.h"

using namespace std;

void HeatFlowActuator::init() {
	if( _initialized )
		return;

	Gpio& gpio = Gpio::getInstance();
	gpio.init();

	pinMode( RELAY_HEATFLOW_PIN, OUTPUT );

	_initialized = true;
	off();

	return;
}

void HeatFlowActuator::deinit() {
	if( !_initialized )
		return;
	
	off();

	Gpio& gpio = Gpio::getInstance();
	gpio.deinit();
	_initialized = false;
	return;
}

void HeatFlowActuator::on() {
	if( !_initialized )
		return;
	
	digitalWrite( RELAY_HEATFLOW_PIN, HIGH );
	_level = LEVEL_ON;

	delay(1);
	return;
}	

void HeatFlowActuator::start( level_t level ) {
	on();
	return;
}

void HeatFlowActuator::off() {
	if( !_initialized )
		return;

	digitalWrite( RELAY_HEATFLOW_PIN, LOW );
	_level = LEVEL_OFF;

	delay(1);
	return;
}

void HeatFlowActuator::stop() {
	off();
	return;
}

level_t HeatFlowActuator::get() {
	if( !_initialized ) 
		return LEVEL_INVALID;

	return _level;
} 

// EOF

