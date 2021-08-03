// HumiditySensor.h

#ifndef __HUMIDITYSENSOR_H__
#define __HUMIDITYSENSOR_H__ 

#include <bool>

class HumiditySensor {
	bool initialized; 

public:
	Humidity() : initialized(false)
	virtual ~Humidity() { initialized = false; }
	void init();
	float get();
};

#endif

// EOF

