// DehumidActuator.cpp
//

#include <wiringPi.h>
#include <softPwm.h>
#include "Gpio.h"
#include "DehumidActuator.h"

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

void DehumidActuator::start() {
	if( !_initialized )
		return;
	
	_level = ( ON / ON * PWM_RANGE2 ) % PWM_RANGE2;
	if( softPwmWrite( PWMFAN2_PIN, ( PWM_RANGE2 - _level ) ) )
		cerr << "softPwmWrite() failed." << endl;

	delay(1);
	return;
}	

void DehumidActuator::start( level_t level ) {
	if( !_initialized )
		return;

	_level = ( level / ON * PWM_RANGE2 ) % PWM_RANGE2; 
	if( softPwmWrite( PWMFAN2_PIN, ( PWM_RANGE2 - _level ) ) )
		cerr << "softPwmWrite() failed." << endl;

	delay(1);
	return;
}

void DehumidActuator::stop() {
	if( !_initialized )
		return;

	_level = ( OFF / ON * PWM_RANGE2 ) % PWM_RANGE2;;
	if( softPwmWrite( PWMFAN2_PIN, ( PWM_RANGE2 - _level ) ) )
		cerr << "softPwmWrite() failed." << endl;

	delay(1);
	return;
}

level_t DehumidActuator::get() {
	if( !_initialized ) 
		return INVALID

	return _level;
} 

// EOF

