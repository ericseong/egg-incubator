// Incubator.cpp
//
// For signal handling, I refered to 
// https://thomastrapp.com/blog/signal-handler-for-multithreaded-c++/.

#include <unistd.h>
#include <iostream>
#include "Env.h"
#include "Incubator.h"

using namespace std;

const string cfgFileName( "config.json" );

int Incubator::_init() {

	// read config file
	_env = &Env::getInstance();

	if( _env->setUp( cfgFileName ) ) {
		cerr << "_env->setUp() failed.\n";
		return -1;
	}
	
	// create instances for sensors and actuators
	_pTempSensor = 			new Tmp117TempSensor();
	_pHumidSensor = 		new Dht22HumidSensor();
	_pAirFlowActuator =	new AirFlowActuator;
	_pDehumidActuator =	new DehumidActuator;
	_pHeatActuator =		new HeatActuator;
	_pRollerActuator =	new RollerActuator;

	clog << "Incubator initialized.\n";
	return 0;
}

int Incubator::_deinit() {
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

	// TODO! write _cfg into config file?
	clog << "Incubator deinitialized.\n";
	return 0;
}

Incubator::Incubator() {
	_init();
}

Incubator::~Incubator() {
	_deinit();
}

void Incubator::breakableLoop() const {

	while( !Signal::doBreak && !Signal::breakRequested.load() ) {
		// TODO! DO THE THINGS HERE!
	
		std::this_thread::sleep_for( std::chrono::seconds(1) );
	}

	// TODO! stop all actuators! 
	
	clog << "Incubator shutting down.\n";
	return;
}

int main() {

	// create singleton incubator instance
	Incubator& inc  = Incubator::getInstance();

	// let's go
	inc.breakableLoop();

	return 0;
}

// EOF

