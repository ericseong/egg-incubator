// Sensor.h

#ifndef __SENSOR_H__
#define __SENSOR_H__

struct Sensor {
	virtual ~Sensor() {}
	virtual void init() = 0; 
	virtual void deinit() = 0; 
	virtual int get( float& data ) const = 0;
};

#endif

// EOF

