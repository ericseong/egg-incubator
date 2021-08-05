// Incubator.cpp
//
// For signal handling, I refered to 
// https://thomastrapp.com/blog/signal-handler-for-multithreaded-c++/.

#include <unistd.h>
#include <iostream>
#include "Env.h"
#include "Incubator.h"

using namespace std;

// my signal handler to break from monitoring loop
void Incubator::signalHandler( int signum ) {

	if( signum == SIGTERM ) {
		Incubator::doBreak = 1;
		Incubator::breakRequested = true;
	}
	if( signum == SIGUSR1 ) {
		// TODO!!
	}

	const char str[] = "Signal received!\n";
	write( STDERR_FILENO, str, sizeof(str)-1 ); // write is signal safe!
	
	exit( signum );
}

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

	// for signal handling
	struct sigaction action;
	action.sa_handler = Incubator::signalHandler;
	sigemptyset( &action.sa_mask );
	action.sa_flags = 0;
	sigaction( SIGTERM, &action, NULL );
	sigaction( SIGUSR1, &action, NULL );

	Incubator::doBreak = 0;
	Incubator::breakRequested = false; //std::atomic is safe, as long as it's lock-free
	assert( breakRequested.is_lock_free() );

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

	while( !doBreak && !breakRequested.load() ) {
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

