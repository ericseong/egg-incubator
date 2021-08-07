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
	if( _pEnv->setUp( cfgFileName ) ) {
		cerr << "_pEnv->setUp() failed.\n";
		return;
	}

	// Instantiate SesssionTime singleton
	_pSTime = &SessionTime::getInstance();
	if( _pSTime->init() ) {
		cerr << "_pSTime->init() failed!.\n";
		return;
	}

	// Instantiate Signal 
	_pSig = new Signal();
	
	// create instances for sensors and actuators
	_pTempSensor = 			new Tmp117TempSensor();
	_pTempSensor->init();

	_pHumidSensor = 		new Dht22HumidSensor();
	_pHumidSensor->init();

	_pAirFlowActuator =	new AirFlowActuator();
	_pAirFlowActuator->init();

	_pDehumidActuator =	new DehumidActuator();
	_pDehumidActuator->init();

	_pHeatActuator =		new HeatActuator();
	_pHeatActuator->init();

	_pRollerActuator =	new RollerActuator();
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

	if( _pSTime ) {
		_pSTime->deinit();
		delete _pSTime;
	}

	clog << "Incubator deinitialized.\n";
	_initialized = false;

	return;
}

void Incubator::newSession() {
	if( !_initialized )
		return;

	_pSTime->	setStart();	
	return;
}

// single shot for incubator control
void Incubator::_run() const {

	if( !_initialized )
	 return;

	// get the control formula from env
	formula_t f;
	//clog << "_pST->daysPassed(): " << _pSTime->daysPassed() << endl;
	if( _pEnv->getFormula( _pSTime->daysPassed(), f ) ) {
	 cerr << "Can't get formula.\n";
	 return;
	}
	
	// temperature control 
	float tm;
	bool airFlowOverridden4TempControl = false;
	if( !_pTempSensor->get( tm ) ) {
		clog << "real-time temperature: " << tm << "oC" << '\n';
		if( tm >= f.tempHigherLimit + 0.1 ) { // temperature too high
			_pAirFlowActuator->start( LEVEL_ON );	
			airFlowOverridden4TempControl = true;
			clog << "airflow actuator ON and airflow is overridden." << '\n';
		}
		if( tm >= f.tempHigherLimit ) {
			_pHeatActuator->off();
			clog << "heat actuator OFF." << '\n';
		}
		else if( tm <= f.tempLowerLimit ) {
			_pHeatActuator->on();
			clog << "heat actuator ON." << '\n';
		}
		else {
			_pHeatActuator->off();
			_pAirFlowActuator->start( f.airFlowLevel );
			airFlowOverridden4TempControl = false;
			clog << "airflow level: " << f.airFlowLevel << "and airflow is to be normal ." << '\n';
		}
	}

	// humidity control 
	float th;
	if( !_pHumidSensor->get( th ) ) {
		clog << "real-time humidity: " << tm << "%" << '\n';
		if( th >= f.humidHigherLimit ) {
			_pDehumidActuator->start( LEVEL_ON );
			clog << "dehumid actuator ON." << '\n';
		}
		else {
			_pDehumidActuator->off();
			clog << "dehumid actuator OFF." << '\n';
		}
	}

	// air flow control
	if( !airFlowOverridden4TempControl ) {
		_pAirFlowActuator->start( f.airFlowLevel );
		clog << "airflow actuator level: " << f.airFlowLevel << '\n';
	}
	
	return;
}

// single shot for roller control
// TODO! better run as a separate thread.
void Incubator::_run4Roller() const {

	if( !_initialized )
		return;

	formula_t f;
	if( _pEnv->getFormula( _pSTime->daysPassed(), f ) )
		return;

	static time_t stamp{0};
	time_t now;
	if( stamp ) {
		time( &stamp );
	} else {
		time( &now );
		if( (now-stamp) > (time_t)f.rollInterval ) {
			clog << "roller actuator starts." << '\n';
			_pRollerActuator->on();
			this_thread::sleep_for( std::chrono::milliseconds(6000) );
			_pRollerActuator->off();	
			clog << "roller actuator ends." << '\n';
			time( &stamp );
		}	
	}

	return;
}

// loop for incubator control
void Incubator::runLoop() const {
	static unsigned count =  0;

	if( !_initialized )
		return;

	while( !Signal::isSignaledTerm() ) {
		if( Signal::isSignaledUsr1() ) {
			_pSTime->deinit();
			_pSTime->init();
			clog << "Incubator got SIGUSR1 and session time is reinitialized.\n";
		}

		_run();
		_run4Roller();

		// some sensors has a limitation on the consecutive reading. dht22 allows to read next at least after two seconds later.
		this_thread::sleep_for( std::chrono::milliseconds(3000) );
		count++;
		clog << "runLoop() with count: " << count << "\n\n";
	}

	// will get here only by SIGTERM
	clog << "Incubator got SIGTERM and is shutting down.\n";

	return;
}

int main( int argc, char *argv[] ) {
 
	// create singleton incubator instance
	Incubator& inc  = Incubator::getInstance();

	// all composited components are initialized.
	inc.init();

	// for new session 
	if( argc == 2 ) {
		string arg = argv[1];
		if( arg.compare( "--new-session" ) == 0 )
			inc.newSession();
	}

	inc.runLoop();
	inc.deinit();

	return 0;
}

// EOF

