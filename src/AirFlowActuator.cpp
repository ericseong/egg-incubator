// AirFlowActuator.cpp
//

#include <iostream>
#include <wiringPi.h>
#include <softPwm.h>
#include "Gpio.h"
#include "AirFlowActuator.h"

#define PWM_RANGE1 100

using namespace std;

void AirFlowActuator::init() {
	if( _initialized )
		return;

	Gpio& gpio = Gpio::getInstance();
	gpio.init();

	if( softPwmCreate( PWMFAN1_PIN, PWM_RANGE1, PWM_RANGE1 ) )
		cerr << "softPwmCreate() failed." << endl;
	else
		_initialized = true;

	return;
}
void AirFlowActuator::start() {
	if( !_initialized )
		return;
	
	_level = ( LEVEL_50 / LEVEL_ON * PWM_RANGE1 ) % PWM_RANGE1;
	softPwmWrite( PWMFAN1_PIN, (PWM_RANGE1 - _level) );

	delay(1);
	return;
}	

void AirFlowActuator::start( level_t level ) {
	if( !_initialized )
		return;

	_level = ( level / LEVEL_ON * PWM_RANGE1 ) % PWM_RANGE1;
	softPwmWrite( PWMFAN1_PIN, ( PWM_RANGE1 - _level ) );

	delay(1);
	return;
}
void AirFlowActuator::stop() {
	if( !_initialized )
		return;

	_level = ( LEVEL_OFF / LEVEL_ON * PWM_RANGE1 ) % PWM_RANGE1;
	softPwmWrite( PWMFAN1_PIN, ( PWM_RANGE1 - _level ) );

	delay(1);
	return;
}

level_t AirFlowActuator::get() {
	if( !_initialized ) 
		return LEVEL_INVALID;

	return _level;
} 

// EOF

