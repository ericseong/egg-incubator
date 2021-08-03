// Dht22TempSensor.h

#ifndef __DHT22TEMPSENSOR_H__
#define __DHT22TEMPSENSOR_H__ 

#include "Sensor.h"

class Dht22TempSensor : public Sensor {
	bool initialized;

public: 
	Dht22TempSensor() : initialized(false) {}
	~Dht22TempSensor() { initialized = false; }
	void init();
	int get( float *data );
};

#endif

// EOF

