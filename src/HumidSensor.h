// HumidSensor.h

#ifndef __HUMIDSENSOR_H__
#define __HUMIDSENSOR_H__ 

class HumidSensor {

public:
	virtual ~HumidSensor();
	virtual void init() = 0;
	virtual int get( float *data ) = 0;
};

#endif

// EOF

