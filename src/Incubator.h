// Incubator.h

#ifndef __INCUBATOR_H__
#define __INCUBATOR_H__

#include <vector>
#include "Signal.h"
#include "Env.h"
#include "SessionTime.h"
#include "Sensor.h"
#include "Actuator.h"
#include "DisplayClient.h"
#include "LockedFileAccess.h"
//#include "Dht22HumidSensor.h"
//#include "Dht22TempSensor.h"
//#include "Tmp117TempSensor.h"
//#include "AirFlowActuator.h"
//#include "DehumidActuator.h"
//#include "HeatActuator.h"
//#include "RollerActuator.h"
#include "TempAlgo.h"
#include "SessionLoggerClient.h"
#include "IncUtil.h"

/*
class Incubator : public Singleton<Incubator> {
	bool _initialized;
	unsigned _runCount;
	Signal *_pSig;
	Env *_pEnv;
	SessionTime *_pSTime;
	DisplayClient *_pDC;
	SessionLoggerClient *_pSLC;
	Sensor *_pTempSensor;
	Sensor *_pHumidSensor;
	Actuator *_pAirFlowActuator;
	Actuator *_pDehumidActuator;
	Actuator *_pHeatActuator;
	Actuator *_pHeatFlowActuator;
	Actuator *_pRollerActuator;
	Actuator *_pHumidActuator;
	TempAlgo *_pTempAlgo;
	void _run() const;
	void _run4Roller() const;
*/

class Incubator : public Singleton<Incubator> {
	bool _initialized;
	unsigned _runCount;
	Signal *_pSig;
	Env *_pEnv;
	SessionTime *_pSTime;
	DisplayClient *_pDC;
	SessionLoggerClient *_pSLC;
	Sensor *_pTempSensor;
	Sensor *_pHumidSensor;
	Actuator *_pAirFlowActuator;
	Actuator *_pDehumidActuator;
	Actuator *_pHeatActuator;
	Actuator *_pHeatFlowActuator;
	Actuator *_pRollerActuator;
	Actuator *_pHumidActuator;
	TempAlgo *_pTempAlgo;
	void _run() const;
	void _run4Roller() const;
	bool _isOon( unsigned margin ) const;
public:
	Incubator() : _initialized(false), _runCount(0) {}
	virtual ~Incubator() {}
	void init();
	void deinit();
	void runLoop();
	void newSession();
	void updatePanel() const;
	void updateSessionLog() const;
	void update4RemoteUI() const;
};

#endif

// EOF

