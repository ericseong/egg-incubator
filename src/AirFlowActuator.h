// AirFlowActuator.h

#ifndef __AIRFLOWACTUATOR_H__
#define __AIRFLOWACTUATOR_H__

#include "Actuator.h"

class AirFlowActuator : public Actuator {
	bool _initialized;
	level_t _level;
	
public:
	AirFlowActuator() : _initialized(false) {}
	virtual ~AirFlowActuator() { _initialized = false; }
	void init();
	void start();
	void start( level_t level );
	void stop();
	level_t get();
};

#endif

