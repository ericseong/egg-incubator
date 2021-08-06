// Dht22HumidSensor.h

#ifndef __DHT22HUMIDSENSOR_H__
#define __DHT22HUMIDSENSOR_H__ 

#include "Sensor.h"

class Dht22HumidSensor : public Sensor {
	bool _initialized;

public:
	Dht22HumidSensor() : _initialized(false) {}
	~Dht22HumidSensor() { _initialized = false; }
	void init();
	void deinit();
	int get( float *data );
};

#endif

// EOF

