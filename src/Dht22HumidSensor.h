// Dht22HumidSensor.h

#ifndef __DHT22HUMIDSENSOR_H__
#define __DHT22HUMIDSENSOR_H__ 

#include "Sensor.h"

class Dht22HumidSensor : public Sensor {
	bool initialized;
public:
	Dht22HumidSensor() : initialized(false) {}
	~Dht22HumidSensor() { initialized = false; }
	void init();
	int get( float *data );
};

#endif

// EOF

