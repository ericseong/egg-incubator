// RollerActuator.h

#ifndef __ROLLERACTUATOR_H__
#define __ROLLERACTUATOR_H__ 

#include "Actuator.h"

class RollerActuator : public Actuator {
	bool _initialized;
  unsigned _level;
	unsigned _count;
	
public:
	RollerActuator() : _initialized(false), _count(0) {}
	virtual ~RollerActuator() { _initialized = false; }
	void init();
	void deinit();
	void on();
	void off();
	void start( level_t );
	void stop();
	level_t get();
	unsigned getCount();
};

#endif

