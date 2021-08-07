// Tmp117TempSensor.h

#ifndef __TMP117TEMPSENSOR_H__
#define __TMP117TEMPSENSOR_H__ 

#include "Sensor.h"

class Tmp117TempSensor : public Sensor {
	bool _initialized;
	int deviceFd;
public: 
	Tmp117TempSensor() : _initialized(false) {}
	~Tmp117TempSensor() { _initialized = false; }
	void init();
	void deinit();
	int get( float *data ) const;
};

#endif

// EOF

