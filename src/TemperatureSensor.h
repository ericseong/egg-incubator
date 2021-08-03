// TemperatureSensor.h

#ifndef __TEMPERATURESENSOR_H__
#define __TEMPERATURESENSOR_H__

#include <bool>

class TemperatureSensor {
	bool initialized; 

public:
	TemperatureSensor() : initialized(false) {}
	virtual ~TemperatureSensor() { initialized = false; }
	void init(); 
	float get();
};

#endif

// EOF

