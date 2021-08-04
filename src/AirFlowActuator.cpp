// AirFlowActuator.cpp
//

#include <wiringPi.h>
#include <softPwm.h>
#include "Gpio.h"
#include "AirFlowActuator.h"

using namespace std;

void AirFlowActuator::init() {
	if( initialized )
		return;

	Gpio& gpio = Gpio::getInstance();
	gpio.init();

	if( softPwmCreate( PWMFAN1_PIN, 100, PWM_RANGE1 ) )
		cerr << "softPwmCreate() failed." << endl;

	return;
}
void AirFlowActuator::start() {
	if( !initialized )
		return;
	
	_level = 50 % PWM_RANGE1;
	if( softPwmWrite( PWMFAN1_PIN, _level % PWM_RANGE1 ) )
		cerr << "softPwmWrite() failed." << endl;

	delay(1);
	return;
}	

void AirFlowActuator::start( level_t level ) {
	if( !initialized )
		return;

	_level = 100-level;
	if( softPwmWrite( PWMFAN1_PIN, _level % PWM_RANGE1 ) )
		cerr << "softPwmWrite() failed." << endl;

	delay(1);
	return;
}
void AirFlowActuator::stop() {
	if( !initialized )
		return;

	_level = 100;
	if( softPwmWrite( PWMFAN1_PIN, _level % PWM_RANGE1 ) )
		cerr << "softPwmWrite() failed." << endl;

	delay(1);
	return;
}

level_t AirFlowActuator::get() {
	if( !initialized ) 
		return INVALID

	return _level;
} 

// EOF

