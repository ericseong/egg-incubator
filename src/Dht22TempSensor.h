// Dht22TempSensor.h

#ifndef __DHT22TEMPSENSOR_H__
#define __DHT22TEMPSENSOR_H__ 

#include "Sensor.h"

class Dht22TempSensor : public Sensor {
	bool _initialized;
	float _lastVal;

public: 
	Dht22TempSensor() : _initialized(false), _lastVal(100.00) {}
	~Dht22TempSensor() { _initialized = false; }
	void init();
	void deinit();
	int get( float& data );
	int getCache( float& data ) const;
};

#endif

// EOF

