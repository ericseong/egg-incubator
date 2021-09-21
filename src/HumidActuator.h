// HumidActuator.h

#ifndef __HUMIDACTUATOR_H__
#define __HUMIDACTUATOR_H__ 

#include "Actuator.h"

class HumidActuator : public Actuator {
	bool _initialized;
	unsigned _level;
	void _press();
	
public:
	HumidActuator() : _initialized(false) {}
	virtual ~HumidActuator() { _initialized = false; }
	void init();
	void deinit();
	void on();
	void off();
	void start( level_t );
	void stop();
	level_t get();
};

#endif

