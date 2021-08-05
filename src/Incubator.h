// Incubator.h

#ifndef __INCUBATOR_H__
#define __INCUBATOR_H__

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
#include <signal.h>
#include <unistd.h>

class Incubator : public Singleton<Incubator> {
	Env &env;
	Sensor *_pTempSensor;
	Sensor *_pHumidSensor;
	Actuator *_pAirFlowActuator;
	Actuator *_pDehumidActuator;
	Actuator *_pHeatActuator;
	Actuator *_pRollerActuator;
	void _init();
	void _deinit();
public:
	Incubator() {}
	virtual ~Incubator() {}
	void breakableLoop() const;

	// for signal handling
	static volatile sig_atomic_t do_break;
	static atomic<bool> break_requested;
	static void signalHandler( int signum );
};

#endif

// EOF
