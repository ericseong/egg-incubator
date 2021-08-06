// Incubator.h

#ifndef __INCUBATOR_H__
#define __INCUBATOR_H__

#include "Signal.h"
#include "Env.h"
#include "SessionTime.h"
#include "Sensor.h"
#include "Actuator.h"
//#include "Dht22HumidSensor.h"
//#include "Dht22TempSensor.h"
//#include "Tmp117TempSensor.h"
//#include "AirFlowActuator.h"
//#include "DehumidActuator.h"
//#include "HeatActuator.h"
//#include "RollerActuator.h"

class Incubator : public Singleton<Incubator> {
	bool _initialized;
	Signal _sig;
	Env *_pEnv;
	SessionTime *_pStime;
	Sensor *_pTempSensor;
	Sensor *_pHumidSensor;
	Actuator *_pAirFlowActuator;
	Actuator *_pDehumidActuator;
	Actuator *_pHeatActuator;
	Actuator *_pRollerActuator;
	void _singleShot();
public:
	Incubator() : _initialized(false) {}
	virtual ~Incubator() {}
	void init();
	void deinit();
	void mainProc() const;
};

#endif

// EOF

