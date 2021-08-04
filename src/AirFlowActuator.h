// AirFlowActuator.h

#ifndef __AIRFLOWACTUATOR_H__
#define __AIRFLOWACTUATOR_H__

#include "Actuator.h"

class AirFlowActuator : public Actuator {
	bool initialized;
	level_t _level;
	
public:
	AirFlowActuator() : initialized(false) {}
	virtual ~AirFlowActuator() { initialized = false; }
	void init();
	void start();
	void start( level_t level );
	void stop();
	level_t get();
}

#endif

