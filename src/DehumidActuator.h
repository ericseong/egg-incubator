// DehumidActuator.h

#ifndef __DEHUMIDACTUATOR_H__
#define __DEHUMIDACTUATOR_H__ 

#include "Actuator.h"

class DehumidActuator : public Actuator {
	bool _initialized;
	level_t _level;
	
public:
	DehumidActuator() : _initialized(false) {}
	virtual ~DehumidActuator() { _initialized = false; }
	void init();
	void start();
	void start( level_t level );
	void stop();
	level_t get();
};

#endif

