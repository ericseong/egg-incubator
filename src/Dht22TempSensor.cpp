// TemperatureSensor.cpp
// how to build: -lwiringPi is required

#include "Dht22TempSensor.h"

using namespace std;

extern "C" {
	extern int dht22_get_data( float *temp, float *humid );
}

void Dht22TempSensor::init() {
	initialized = true;
	return;
}

int Dht22TempSensor::get( float *data ) {
	int ret = 0;
	float temp, humid;

	if( !dht22_get_data( &temp, &humid ) )
		*data = temp;
	else
		ret = -1;

	return ret;
}

// EOF

