// HumidActuator.cpp
//

#include <wiringPi.h>
#include "Gpio.h"
#include "HumidActuator.h"

using namespace std;

void HumidActuator::init() {
	if( _initialized )
		return;

	Gpio& gpio = Gpio::getInstance();
	gpio.init();

	pinMode( RELAY_HUMIDIFIER_POWER_PIN, OUTPUT );
	delay(1);

	// turn off humidifier 
	digitalWrite( RELAY_HUMIDIFIER_POWER_PIN, HIGH );
	delay(1);

	pinMode( RELAY_HUMIDIFIER_PIN, OUTPUT );

	_initialized = true;
	off();

	return;
}

void HumidActuator::deinit() {
	if( !_initialized )
		return;
	
	off();

	// turn off humidifier 
	digitalWrite( RELAY_HUMIDIFIER_POWER_PIN, LOW );
	delay(1);

	Gpio& gpio = Gpio::getInstance();
	gpio.deinit();

	_initialized = false;
	return;
}

void HumidActuator::start( level_t level ) {
	if( !_initialized )
		return;
	
	level_t curLevel = get();

	if( level == LEVEL_ON ) {
		switch( curLevel ) {
			case LEVEL_ON:
				break;
			case LEVEL_50:
				_press();
				break;
			case LEVEL_OFF:
				_press();
				_press();
				break;
			default:
				// nothing to do
				break;
		}
	} else if( level == LEVEL_50 )  {
		switch( curLevel ) {
			case LEVEL_ON:
				_press();
				_press();
				break;
			case LEVEL_50:
				break;
			case LEVEL_OFF:
				_press();
				break;
			default:
				// nothing to do
				break;
		}
	} else if( level == LEVEL_OFF ) {
		switch( curLevel ) {
			case LEVEL_ON:
				_press();
				break;
			case LEVEL_50:
				_press();
				_press();
				break;
			case LEVEL_OFF:
				break;
			default:
				// nothing to do
				break;
	} else {
		;
	}

	_level = level;

	return;
}

void HumidActuator::stop() {
	if( !_initialized )
		return;

	start( LEVEL_OFF );

	return;
}

void HumidActuator::on() {
	if( !_initialized )
		return;

	start( LEVEL_ON );

	return;
}

void HumidActuator::off() {
	if( !_initialized )
		return;

	start( LEVEL_OFF );

	return;
}

level_t HumidActuator::get() {
	if( !_initialized ) 
		return LEVEL_INVALID;

	return _level;
} 

// simulate one button press
void HumidActuator::_press() {
	delay(1);
	digitalWrite( RELAY_HUMIDIFIER_POWER_PIN, HIGH );
	delay(1);
	digitalWrite( RELAY_HUMIDIFIER_POWER_PIN, LOW );
	delay(1);

	return;
}

// EOF

