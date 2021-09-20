// TemperatureSensor.cpp
// how to build: -lwiringPi is required

#include <wiringPi.h>
#include "Gpio.h"
#include "Dht22TempSensor.h"

using namespace std;

extern int dht22_get_data( float *temp, float *humid );

void Dht22TempSensor::init() {
	if( _initialized )
		return;

	Gpio& gpio = Gpio::getInstance();
	gpio.init();

	_initialized = true;
	return;
}

void Dht22TempSensor::deinit() {
  if( !_initialized )
    return;

	Gpio& gpio = Gpio::getInstance();
	gpio.deinit();

  _initialized = false;
  return;
}

int Dht22TempSensor::get( float& data ) {
	int ret = 0;
	float temp, humid;

	if( !_initialized )
		return -1;

	if( !dht22_get_data( &temp, &humid ) ) {
		_lastVal = data = temp;
	}
	else {
		ret = -1;
	}

	return ret;
}

int Dht22TempSensor::getCache( float& data ) const {
	if( !_initialized )
		return -1;

	data = _lastVal;
	return 0;
}

// EOF

