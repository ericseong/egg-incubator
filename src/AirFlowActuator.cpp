// AirFlowActuator.cpp
//

#include <iostream>
#include <wiringPi.h>
#include <softPwm.h>
#include "Gpio.h"
#include "AirFlowActuator.h"

#define PWM_RANGE1 LEVEL_ON 

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

void AirFlowActuator::deinit() {
	if( !_initialized )
		return;

	off();

	softPwmStop ( PWMFAN1_PIN );

	Gpio& gpio = Gpio::getInstance();
	gpio.deinit();
	_initialized = false;
	return;
}

void AirFlowActuator::on() {
	if( !_initialized )
		return;
	
	_level = LEVEL_50; // arbitary default, will be set by config
	softPwmWrite( PWMFAN1_PIN, (PWM_RANGE1 - _level) );
	//clog << "softPwmWrite(on): " << ( PWM_RANGE1 - _level ) << endl;

	delay(1);
	return;
}	

void AirFlowActuator::start( level_t level ) {
	if( !_initialized )
		return;

	_level = level;
	softPwmWrite( PWMFAN1_PIN, ( PWM_RANGE1 - _level ) );
	//clog << "softPwmWrite(level): " << ( PWM_RANGE1 - _level ) << endl;

	delay(1);
	return;
}
void AirFlowActuator::off() {
	if( !_initialized )
		return;

	_level = LEVEL_OFF;
	softPwmWrite( PWMFAN1_PIN, ( PWM_RANGE1 - _level ) );
	//clog << "softPwmWrite(off): " << ( PWM_RANGE1 - _level ) << endl;

	delay(1);
	return;
}

level_t AirFlowActuator::get() {
	if( !_initialized ) 
		return LEVEL_INVALID;

	return _level;
} 

// EOF

