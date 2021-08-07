// HeatActuator.h

#ifndef __HEATACTUATOR_H__
#define __HEATACTUATOR_H__ 

#include "Actuator.h"

class HeatActuator : public Actuator {
	bool _initialized;
	//unsigned _level;
	
public:
	HeatActuator() : _initialized(false) {}
	virtual ~HeatActuator() { _initialized = false; }
	void init();
	void deinit();
	void on();
	void off();
	//level_t get();
};

#endif

