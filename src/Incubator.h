/ Incubator.h

#ifndef __INCUBATOR_H__
#define __INCUBATOR_H__

#include "Signal.h"
#include "Singleton.h"
#include "Dht22HumidSensor.h"
#include "Dht22TempSensor.h"
#include "Tmp117TempSensor.h"
#include "AirFlowActuator.h"
#include "DehumidActuator.h"
#include "HeatActuator.h"
#include "RollerActuator.h"

// for signal handler
#include <atomic>
#include <chrono>
#include <thread>
#include <cassert>
#include <signal.h>
#include <unistd.h>

class Incubator : public Singleton<Incubator> {
	Signal _sig;
	Env *_env;
	SessionTime *_stime;
	Sensor *_pTempSensor;
	Sensor *_pHumidSensor;
	Actuator *_pAirFlowActuator;
	Actuator *_pDehumidActuator;
	Actuator *_pHeatActuator;
	Actuator *_pRollerActuator;
	int _init();
	int _deinit();
public:
	Incubator();
	virtual ~Incubator();
	void MainProc() const;
};

#endif

// EOF

