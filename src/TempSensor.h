// TempSensor.h

#ifndef __TEMPSENSOR_H__
#define __TEMPSENSOR_H__

class TempSensor {

public:
	virtual ~TempSensor();
	virtual void init() = 0; 
	virtual int get( float *data ) = 0;
};

#endif

// EOF

