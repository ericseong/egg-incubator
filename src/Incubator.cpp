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

// single shot for incubator control
void Incubator::_run() const {

	if( !_initialized )
	 return;

	// get the control formula from env
	formula_t f;
	if( _pEnv->getFormula( _pStime->dayPassed(), f ) ) {
	 cout << "Can't get formula.\n";	
	 return;
	}
	
	// temperature control 
	float tm;
	bool airFlowOverridden4TempControl = false;
	if( !_pTempSensor->get( tm ) ) {
		if( tm >= f.tempHigherLimit + 0.1 ) { // temperature too high
			_pAirFlowActuator->start( LEVEL_ON );	
			airFlowOverridden4TempControl = true;
		}
		if( tm >= f.tempHigherLimit )
			_pHeatActuator->stop();
		else if( tm <= f.tempLowerLimit )
			_pHeatActuator->start();
		else {
			_pHeatActuator->stop();
			_pAirFlowActuator->start( f.airFlowLevel );
			airFlowOverridden4TempControl = false;
		}
	}

	// humidity control 
	float th;
	if( !_pHumidSensor->get( th ) ) {
		if( th >= f.humidHigherLimit )
			_pDehumidActuator->start( LEVEL_ON );
		else
			_pDehumidActuator->stop();
	}

	// air flow control
	if( !airFlowOverridden4TempControl ) {
		_pAirFlowActuator->start( f.airFlowLevel );
	}
	
	return;
}

// single shot for roller control
// TODO! better run this in a separate thread.
void Incubator::_run4Roller() const {

	if( !_initialized )
		return;

	formula_t f;
	if( _pEnv->getFormula( _pStime->dayPassed(), f ) )
		return;

	static time_t stamp{0};
	time_t now;
	if( stamp ) {
		time( &stamp );
	} else {
		time( &now );
		if( (now-stamp) > f.rollInterval ) {
			_pRollerActuator->start();	
			this_thread::sleep_for( std::chrono::milliseconds(6000) );
			_pRollerActuator->stop();	
			time( &stamp );
		}	
	}

	return;
}

// loop for incubator control
void Incubator::runLoop() const {

	if( !_initialized )
		return;

	while( !Signal::isSignaledTerm() ) {
		if( Signal::isSignaledUsr1() ) {
			_pStime->deinit();
			_pStime->init();
		}

		_run();
		_run4Roller();

		this_thread::sleep_for( std::chrono::seconds(5) );
	}

	// only get here by SIGTERM
	clog << "Incubator shutting down.\n";

	return;
}

int main() {

	// create singleton incubator instance
	Incubator& inc  = Incubator::getInstance();

	inc.init();
	inc.runLoop();
	inc.deinit();

	return 0;
}

// EOF

