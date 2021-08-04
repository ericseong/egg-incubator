// HumidActuator.cpp
//

#include <wiringPi.h>
#include <softPwm.h>
#include "Gpio.h"
#include "HumidActuator.h"

using namespace std;

void HumidActuator::init() {
	if( initialized )
		return;

	Gpio& gpio = Gpio::getInstance();
	gpio.init();

	if( softPwmCreate( PWMFAN2_PIN, 100, PWM_RANGE2 ) )
		cerr << "softPwmCreate() failed." << endl;

	return;
}
void HumidActuator::start() {
	if( !initialized )
		return;
	
	_level = 50 % PWM_RANGE2;
	if( softPwmWrite( PWMFAN2_PIN, _level % PWM_RANGE2 ) )
		cerr << "softPwmWrite() failed." << endl;

	delay(1);
	return;
}	

void HumidActuator::start( level_t level ) {
	if( !initialized )
		return;

	_level = 100-level;
	if( softPwmWrite( PWMFAN2_PIN, _level % PWM_RANGE2 ) )
		cerr << "softPwmWrite() failed." << endl;

	delay(1);
	return;
}
void HumidActuator::stop() {
	if( !initialized )
		return;

	_level = 100;
	if( softPwmWrite( PWMFAN2_PIN, _level % PWM_RANGE2 ) )
		cerr << "softPwmWrite() failed." << endl;

	delay(1);
	return;
}

level_t HumidActuator::get() {
	if( !initialized ) 
		return INVALID

	return _level;
} 

// EOF

