// Dht22TempSensor.h

#ifndef __DHT22TEMPSENSOR_H__
#define __DHT22TEMPSENSOR_H__ 

#include "TempSensor.h"

class Dht22TempSensor : public TempSensor {
	bool initialized;

public: 
	Dht22TempSensor() : initialized(false) {}
	~Dht22TempSensor() { initialized = false; }
	void init();
	int get( float *data );
};

#endif

// EOF

