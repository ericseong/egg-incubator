// Sensor.h

#ifndef __SENSOR_H__
#define __SENSOR_H__

class Sensor {

public:
	virtual ~Sensor() {}
	virtual void init() = 0; 
	virtual int get( float *data ) = 0;
};

#endif

// EOF
