// RollerActuator.h

#ifndef __ROLLERACTUATOR_H__
#define __ROLLERACTUATOR_H__ 

#include "Actuator.h"

class RollerActuator : public Actuator {
	bool _initialized;
	
public:
	RollerActuator() : _initialized(false) {}
	virtual ~RollerActuator() { _initialized = false; }
	void init();
	void deinit();
	void on();
	void off();
	void start( level_t );
	void stop();
	level_t get();
};

#endif

