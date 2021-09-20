// HeatFlowActuator.h

#ifndef __HEATFLOWACTUATOR_H__
#define __HEATFLOWACTUATOR_H__

#include "Actuator.h"

class HeatFlowActuator : public Actuator {
	bool _initialized;
	unsigned _level;
	
public:
	HeatFlowActuator() : _initialized(false) {}
	virtual ~HeatFlowActuator() { _initialized = false; }
	void init();
	void deinit();
	void on();
	void off();
	void start( level_t );
	void stop();
	level_t get();
};

#endif

