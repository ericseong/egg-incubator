// AirFlowActuator.h

#ifndef __AIRFLOWACTUATOR_H__
#define __AIRFLOWACTUATOR_H__

#include "Actuator.h"

class AirFlowActuator : public Actuator {
	ActStatus status;
	
public:
	void set( ActStatus status );
	ActStatus get();
	void start();
	void startHigh();
	void startMid();
	void startLow();
	void stop();
}

#endif

