// DehumidActuator.cpp
//

#include <iostream>
#include <wiringPi.h>
#include <softPwm.h>
#include "Gpio.h"
#include "DehumidActuator.h"

#define PWM_RANGE2 100

using namespace std;

void DehumidActuator::init() {
	if( _initialized )
		return;

	Gpio& gpio = Gpio::getInstance();
	gpio.init();

	softPwmCreate( PWMFAN2_PIN, PWM_RANGE2, PWM_RANGE2 );
	_initialized = true;

	return;
}

void DehumidActuator::deinit() {
	stop();
	return;
}

void DehumidActuator::start() {
	if( !_initialized )
		return;
	
	_level = ( LEVEL_ON / LEVEL_ON * PWM_RANGE2 ) % PWM_RANGE2;
	softPwmWrite( PWMFAN2_PIN, ( PWM_RANGE2 - _level ) );

	delay(1);
	return;
}	

void DehumidActuator::start( level_t level ) {
	if( !_initialized )
		return;

	_level = ( level / LEVEL_ON * PWM_RANGE2 ) % PWM_RANGE2; 
	softPwmWrite( PWMFAN2_PIN, ( PWM_RANGE2 - _level ) );

	delay(1);
	return;
}

void DehumidActuator::stop() {
	if( !_initialized )
		return;

	_level = ( LEVEL_OFF / LEVEL_ON * PWM_RANGE2 ) % PWM_RANGE2;;
	softPwmWrite( PWMFAN2_PIN, ( PWM_RANGE2 - _level ) );

	delay(1);
	return;
}

level_t DehumidActuator::get() {
	if( !_initialized ) 
		return LEVEL_INVALID;

	return _level;
} 

// EOF

