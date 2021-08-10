// Tmp117TempSensor.h

#ifndef __TMP117TEMPSENSOR_H__
#define __TMP117TEMPSENSOR_H__ 

#include "Sensor.h"

class Tmp117TempSensor : public Sensor {
	bool _initialized;
	int _deviceFd;
	float _lastVal;
public: 
	Tmp117TempSensor() : _initialized(false), _lastVal(100.00) {}
	~Tmp117TempSensor() { _initialized = false; }
	void init();
	void deinit();
	int get( float& data );
	int getCache( float& data ) const;
};

#endif

// EOF

