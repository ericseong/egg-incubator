// Incubator.cpp
//
// For signal handling, I refered to 
// https://thomastrapp.com/blog/signal-handler-for-multithreaded-c++/.

#include <unistd.h>
#include <iostream>
#include "Incubator.h"
#include "Tmp117TempSensor.h"
#include "Dht22HumidSensor.h"
#include "AirFlowActuator.h"
#include "DehumidActuator.h"
#include "HeatActuator.h"
#include "RollerActuator.h"

using namespace std;

const string cfgFileName( "config.json" );

void Incubator::init() {

	if( _initialized )
		return;

	// init Env singleton
	_pEnv = &Env::getInstance();
	if( !_pEnv->setUp( cfgFileName ) ) {
		cerr << "_pEnv->setUp() failed.\n";
		return;
	}

	// init SesssionTime singleton
	_pStime = &SessionTime::getInstance();
	if( !_pStime->init() ) {
		cerr << "_pStime->init() failed!.\n";
		return;
	}
	
	// create instances for sensors and actuators
	_pTempSensor = 			new Tmp117TempSensor();
	_pHumidSensor = 		new Dht22HumidSensor();
	_pAirFlowActuator =	new AirFlowActuator;
	_pDehumidActuator =	new DehumidActuator;
	_pHeatActuator =		new HeatActuator;
	_pRollerActuator =	new RollerActuator;

	clog << "Incubator initialized.\n";
	_initialized = true;
	return;
}

void Incubator::deinit() {
	if( !_initialized )
		return;

	if( _pTempSensor )
		delete _pTempSensor;
	if( _pHumidSensor )
		delete _pHumidSensor;
	if( _pAirFlowActuator )
		delete _pAirFlowActuator;
	if( _pDehumidActuator )
		delete _pDehumidActuator;
	if( _pHeatActuator )
		delete _pHeatActuator;
	if( _pRollerActuator )
		delete _pRollerActuator;

	_pStime->deinit();

	// stop all actuators! 
	_pAirFlowActuator->stop();
	_pDehumidActuator->stop();
	_pHeatActuator->stop();
	_pRollerActuator->stop();
	
	clog << "Incubator deinitialized.\n";
	_initialized = false;
	return;
}

void Incubator::_singleShot() {
	return;
}

void Incubator::mainProc() const {

	while( !Signal::isSignaledTerm() ) {

		if( Signal::isSignaledUsr1() ) {
			_pStime->deinit();
			_pStime->init();
		}

		// TODO! DO THE THINGS HERE!
	
		std::this_thread::sleep_for( std::chrono::seconds(1) );
	}

	clog << "Incubator shutting down.\n";
	return;
}

int main() {

	// create singleton incubator instance
	Incubator& inc  = Incubator::getInstance();

	inc.init();
	inc.mainProc();
	inc.deinit();

	return 0;
}

// EOF

