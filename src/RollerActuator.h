// RollerActuator.h

#ifndef __ROLLERACTUATOR_H__
#define __ROLLERACTUATOR_H__ 

#include "Actuator.h"

class RollerActuator : public Actuator {
	bool _initialized;
	unsigned _level;
	
public:
	RollerActuator() : _initialized(false) {}
	virtual ~RollerActuator() { _initialized = false; }
	void init();
	void start();
	void stop();
	level_t get();
};

#endif

