// AirFlowActuator.cpp
//

#include <wiringPi.h>
#include <softPwm.h>
#include "Gpio.h"
#include "AirFlowActuator.h"

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
	
	_level = ( LEVEL_50 / ON * PWM_RANGE1 ) % PWM_RANGE1;
	if( softPwmWrite( PWMFAN1_PIN, (PWM_RANGE1 - _level) ) )
		cerr << "softPwmWrite() failed." << endl;

	delay(1);
	return;
}	

void AirFlowActuator::start( level_t level ) {
	if( !_initialized )
		return;

	_level = ( level / ON * PWM_RANGE1 ) % PWM_RANGE1;
	if( softPwmWrite( PWMFAN1_PIN, ( PWM_RANGE1 - _level ) ) )
		cerr << "softPwmWrite() failed." << endl;

	delay(1);
	return;
}
void AirFlowActuator::stop() {
	if( !_initialized )
		return;

	_level = ( OFF / ON * PWM_RANGE1 ) % PWM_RANGE1;
	if( softPwmWrite( PWMFAN1_PIN, ( PWM_RANGE1 - _level ) ) )
		cerr << "softPwmWrite() failed." << endl;

	delay(1);
	return;
}

level_t AirFlowActuator::get() {
	if( !_initialized ) 
		return INVALID

	return _level;
} 

// EOF

