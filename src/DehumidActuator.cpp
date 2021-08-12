// DehumidActuator.cpp
//

#include <iostream>
#include <wiringPi.h>
#include <softPwm.h>
#include "Gpio.h"
#include "DehumidActuator.h"

#define PWM_RANGE2 LEVEL_ON 

using namespace std;

void DehumidActuator::init() {
	if( _initialized )
		return;

	Gpio& gpio = Gpio::getInstance();
	gpio.init();

  if( softPwmCreate( PWMFAN2_PIN, PWM_RANGE2, PWM_RANGE2 ) )
    cerr << "softPwmCreate() failed." << endl;
  else
    _initialized = true;

	return;
}

void DehumidActuator::deinit() {
	if( !_initialized )
		return;

	off();
	softPwmStop ( PWMFAN2_PIN );

	Gpio& gpio = Gpio::getInstance();
	gpio.deinit();
  _initialized = false;
	return;
}

void DehumidActuator::on() {
	if( !_initialized )
		return;
	
	_level = LEVEL_ON;
	softPwmWrite( PWMFAN2_PIN, ( PWM_RANGE2 - _level ) );
	//clog << "softPwmWrite(on): " << ( PWM_RANGE2 - _level ) << endl; 

	delay(1);
	return;
}	

void DehumidActuator::start( level_t level ) {
	if( !_initialized )
		return;

	_level = level; 
	softPwmWrite( PWMFAN2_PIN, ( PWM_RANGE2 - _level ) );
	//clog << "softPwmWrite(level): " << ( PWM_RANGE2 - _level ) << endl; 

	delay(1);
	return;
}

void DehumidActuator::off() {
	if( !_initialized )
		return;

	_level = LEVEL_OFF;
	softPwmWrite( PWMFAN2_PIN, ( PWM_RANGE2 - _level ) );
	//clog << "softPwmWrite(off): " << ( PWM_RANGE2 - _level ) << endl; 

	delay(1);
	return;
}

level_t DehumidActuator::get() {
	if( !_initialized ) 
		return LEVEL_INVALID;

	return _level;
} 

// EOF

