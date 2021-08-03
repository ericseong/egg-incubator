// Tmp117TempSensor.h

#ifndef __TMP117TEMPSENSOR_H__
#define __TMP117TEMPSENSOR_H__ 

#include "Sensor.h"

class Tmp117TempSensor : public Sensor {
	bool initialized;
	int deviceFd;
public: 
	Tmp117TempSensor() : initialized(false) {}
	~Tmp117TempSensor() { initialized = false; }
	void init();
	int get( float *data );
};

#endif

// EOF

