// Incubator.cpp
//
// For signal handling, I refered to 
// https://thomastrapp.com/blog/signal-handler-for-multithreaded-c++/.


#include <unistd.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
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

#if 1
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
#endif

#if 0
	// Instantiate InfoPanel
	// ERIC 
	_pInfoPanel = new InfoPanel();
	// ERIC 
	_pInfoPanel->init();
#endif
	
#if 1
	// create instances for sensors and actuators
	_pTempSensor = 			new Tmp117TempSensor();
	_pTempSensor->init();

	_pHumidSensor = 		new Dht22HumidSensor();
	_pHumidSensor->init();

	_pHeatActuator =		new HeatActuator();
	_pHeatActuator->init();

	_pRollerActuator =	new RollerActuator();
	_pRollerActuator->init();

	_pAirFlowActuator =	new AirFlowActuator();
	_pAirFlowActuator->init();

	_pDehumidActuator =	new DehumidActuator();
	_pDehumidActuator->init();

	string host("127.0.0.1");
	_pDC = new DisplayClient( host, 48557 ); 
	//_pDC->init(); // there's no init() in DisplayClient

	// Instantiate Signal 
	_pSig = new Signal();

#endif
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
		clog << "_pTempSensor is deinitialized.\n";
	}
	if( _pHumidSensor ) {
		_pHumidSensor->deinit();	
		delete _pHumidSensor;
		clog << "_pHumidSensor is deinitialized.\n";
	}
	if( _pAirFlowActuator ) { 
		_pAirFlowActuator->deinit();	
		delete _pAirFlowActuator;
		clog << "_pAirFlowActuator is deinitialized.\n";
	}
	if( _pDehumidActuator ) {
		_pDehumidActuator->deinit();	
		delete _pDehumidActuator;
		clog << "_pDehumidActuator is deinitialized.\n";
	}
	if( _pHeatActuator ) {
		_pHeatActuator->deinit();	
		delete _pHeatActuator;
		clog << "_pHeatActuator is deinitialized.\n";
	}
	if( _pRollerActuator ) {
		_pRollerActuator->deinit();	
		delete _pRollerActuator;
		clog << "_pRollerActuator is deinitialized.\n";
	}

	if( _pSTime ) {
		_pSTime->deinit();
		//delete _pSTime; // singleton shall be destroyed at program exit.
		clog << "_pSTime is deinitialized.\n";
	}

	if( _pEnv ) {
		//delete _pEnv; // singleton shall be destroyed at program exit.
		clog << "_pEnv is deinitialized.\n";
	}

	if( _pDC ) {
		//_pDC->deinit(); // there's no deinit() in DisplayClient
		delete _pDC;
		clog << "_pDC is deinitialized.\n";
	}

	if( _pSig ) {
		//delete _pSig; // singleton shall be destroyed at program exit.
		clog << "_pSig is deinitialized.\n";
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
	unsigned daysPassed = _pSTime->daysPassed();
	
	if( _pEnv->getFormula( daysPassed, f ) ) {
	 cerr << "Can't get formula.\n";
	 return;
	} else {
		//clog << "### Days passed: " << daysPassed << endl;
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
		clog << "real-time humidity: " << th << "%" << '\n';
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
	if( !stamp ) {
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

// from lib/GUI/GUI_Paint.h of 1.3" LCD HAT library
#define WHITE          0xFFFF
#define BLACK          0x0000
#define BLUE           0x001F
#define RED            0xF800

#if 0
void Incubator::updatePanel() const {
	_pInfoPanel->drawInfoPanel( "Hi~", WHITE, "Hello", RED, "world!", BLUE );
}
#endif

#if 1
// show stats on lcd
void Incubator::updatePanel() const {

	clog << "1\n";
	// for panel header
	unsigned daysPassed = _pSTime->daysPassed();
	formula_t f;
	if( _pEnv->getFormula( daysPassed, f ) )
		return;

	clog << "2\n";
	string header;
	header = to_string( daysPassed );
  header.append( " days passed. runLoop count: " );
  header.append( to_string( _runCount ) );

	clog << "3\n";
	// for info1 - temperature 
	string info1("Temperature: ");
	float temp;
	if( _pTempSensor->getCache( temp ) )
		temp = 100.00;
	ostringstream oss1;
	oss1.precision( 2 );
	oss1 << std::fixed << temp << " oC";
	info1.append( oss1.str() );
	oss1.clear();

	clog << "4\n";
	// for info2 - humidity 
	string info2("Humidity: ");
	float humid;
	if( _pHumidSensor->getCache( humid ) )
		humid = 100.00;
	ostringstream oss2;
	oss2.precision( 2 );
	oss2 << std::fixed << humid << " %";
	info2.append( oss2.str() );
	oss2.clear();
 
	clog << "5\n";
	// for info3 - reserved
	string info3("");

	clog << "6\n";
	// for footer
	string footer("Last update: ");
	time_t now; time(&now);
	struct tm *timeptr;
	timeptr = localtime(&now);
	char s[100];
	strftime( s, sizeof(s), "%A %b %d %r", timeptr );
	footer.append( s );
	
	clog << "7\n";
	string headerColor("white");
	string info1Color("white");
	string info2Color("white");
	string info3Color("white");
	string footerColor("white");
	if( temp > f.tempHigherLimit )
		info1Color = "red";
	else if( temp < f.tempLowerLimit )
		info1Color = "blue";
	if( humid > f.humidHigherLimit )
		info2Color = "red";
	else if( humid < f.humidLowerLimit )
		info2Color = "blue";

	clog << "8\n";
	// a whole chunk of msg formatting
	std::stringstream ss;
	ss << 
		header << "$" << headerColor << "$" <<
		info1 << "$" << info1Color << "$" <<
		info2 << "$" << info2Color << "$" <<
		info3 << "$" << info3Color << "$" <<
		footer << "$" << footerColor
	;
	std::string msg = ss.str();
	msg.append('\0');
	_pDC->sendMsg( msg ); 
	clog << "Incubator: pDC->sendMsg(): " << msg << endl;

	return;
}
#endif

// loop for incubator control
void Incubator::runLoop() {
	_runCount =  0;

	if( !_initialized )
		return;

#if 0
	while( 1 ) {
#endif
#if 1
	while( !Signal::isSignaledTerm() ) {
		if( Signal::isSignaledUsr1() ) {
			_pSTime->deinit();
			_pSTime->init();
			Signal::resetSignalUsr1();
			clog << "Incubator got SIGUSR1 and session time is reinitialized.\n";
		}

		unsigned daysPassed = _pSTime->daysPassed();
		unsigned maxDay;
		_pEnv->getMaxDay( maxDay );
		if( daysPassed >= maxDay ) {
			clog << "Max day passed: " << "maxDay: " << maxDay << ", " << "daysPassed: " << daysPassed << endl;
			return;
		}

		_runCount++;
		clog << "\n[" << _runCount << "] " << _pSTime->daysPassed() << " days passed or " << _pSTime->getElapsed() << " ticks elapsed.\n";

		_run();
		_run4Roller();

#endif
		updatePanel();

		// some sensors has a limitation on the consecutive reading. dht22 allows to read next at least after two seconds later.
		this_thread::sleep_for( std::chrono::milliseconds(3000) );

		//clog << '\n';
	}

#if 1
	// will get here only by SIGTERM
	Signal::resetSignalTerm(); // needed?
	clog << "Incubator got SIGTERM and is shutting down.\n";
#endif

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

