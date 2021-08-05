// Incubator.cpp
#include <unistd.h>
#include <iostream>
#include "Incubator.h"

using namespace std;

static Incubator& inc  = Incubator::getInstance();

void Incubator::_init() {
	 env = Env::getInstance();

	 // TODO! get config

 	_pTempSensor = 			new Tmp117TempSensor();
	_pHumidSensor = 		new Dht22HumidSensor();
	_pAirFlowActuator =	new AirFlowActuator;
	_pDehumidActuator =	new DehumidActuator;
	_pHeatActuator =		new HeatActuator;
	_pRollerActuator =	new RollerActuator;

	return;
}

void Incubator::_deinit() {
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

	return;
}

Incubator::Incubator() {
	_init();
}

Incubator::~Incubator() {
	_deinit();
}

void Incubator::BreakableLoop() const {

	// TODO! how to exit this loop by signal handler?
	while( true ) {
		cout << "going to sleep..\n";
		sleep(1);
	}

	// TODO! stop all actuators! 
	
	return;
}

void signalHandler( int signum ) {
	clog << "Signal received: (" << signum << ") received.\n";

	// stop the actuators before termination
	// TODO!
	
	exit( signum );
}

int main() {

	// will get SIGTERM by "systemctl stop or restart"
	signal( SIGTERM, signalHandler );

	inc.start();

	return 0;
}

// EOF

