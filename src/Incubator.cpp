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

	// Instantiate Env singleton
	_pEnv = &Env::getInstance();
	if( !_pEnv->setUp( cfgFileName ) ) {
		cerr << "_pEnv->setUp() failed.\n";
		return;
	}

	// Instantiate SesssionTime singleton
	_pStime = &SessionTime::getInstance();
	if( !_pStime->init() ) {
		cerr << "_pStime->init() failed!.\n";
		return;
	}

	// Instantiate Signal 
	_pSig = new Signal();
	
	// create instances for sensors and actuators
	_pTempSensor = 			new Tmp117TempSensor();
	_pTempSensor->init();

	_pHumidSensor = 		new Dht22HumidSensor();
	_pHumidSensor->init();

	_pAirFlowActuator =	new AirFlowActuator;
	_pAirFlowActuator->init();

	_pDehumidActuator =	new DehumidActuator;
	_pDehumidActuator->init();

	_pHeatActuator =		new HeatActuator;
	_pHeatActuator->init();

	_pRollerActuator =	new RollerActuator;
	_pRollerActuator->init();

	clog << "Incubator initialized.\n";
	_initialized = true;

	return;
}

void Incubator::deinit() {
	if( !_initialized )
		return;
	
	if( _pTempSensor ) {
		_pTempSensor->deinit();	
		delete _pTempSensor;
	}
	if( _pHumidSensor ) {
		_pHumidSensor->deinit();	
		delete _pHumidSensor;
	}
	if( _pAirFlowActuator ) { 
		_pAirFlowActuator->deinit();	
		delete _pAirFlowActuator;
	}
	if( _pDehumidActuator ) {
		_pDehumidActuator->deinit();	
		delete _pDehumidActuator;
	}
	if( _pHeatActuator ) {
		_pHeatActuator->deinit();	
		delete _pHeatActuator;
	}
	if( _pRollerActuator ) {
		_pRollerActuator->deinit();	
		delete _pRollerActuator;
	}

	if( _pSig ) {
		delete _pSig;
	}

	if( _pStime ) {
		_pStime->deinit();
		delete _pStime;
	}

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

