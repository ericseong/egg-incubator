// Dht22HumidSensor.cpp

#include "Dht22HumidSensor.h"
using namespace std;

//extern "C" {
//	extern int dht22_get_data( float *temp, float *humid );
//}
extern int dht22_get_data( float *temp, float *humid );

void Dht22HumidSensor::init() {

	if( _initialized )
		return;

	_initialized = true;
	return;
}

void Dht22HumidSensor::deinit() {
	if( !_initialized )
		return;

	_initialized = false;
	return;
}

int Dht22HumidSensor::get( float *data ) {
	int ret = 0;
	float temp, humid;

	if( !_initialized )
		return -1;

	if( !dht22_get_data( &temp, &humid ) )
		*data = humid;
	else
		ret = -1;

	return ret;
}

// EOF

