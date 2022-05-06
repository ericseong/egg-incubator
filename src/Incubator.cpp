// Incubator.cpp
//
// For signal handling, I refered to 
// https://thomastrapp.com/blog/signal-handler-for-multithreaded-c++/.

#include <systemd/sd-daemon.h> // for sd_notify
#include <stdint.h>
#include <unistd.h>
#include <string.h>
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
#include "HeatFlowActuator.h"
#include "RollerActuator.h"
#include "HumidActuator.h"
#include "TempAlgo.h" // added for slow temp. ramp-up

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

	// create instances for sensors and actuators
	_pTempSensor = new Tmp117TempSensor();
	_pTempSensor->init();

	_pHumidSensor =	new Dht22HumidSensor();
	_pHumidSensor->init();

	_pHeatActuator = new HeatActuator();
	_pHeatActuator->init();

	_pHeatFlowActuator = new HeatFlowActuator();
	_pHeatFlowActuator->init();

	_pRollerActuator = new RollerActuator();
	_pRollerActuator->init();

	_pAirFlowActuator =	new AirFlowActuator();
	_pAirFlowActuator->init();

	_pDehumidActuator =	new DehumidActuator();
	_pDehumidActuator->init();

	_pHumidActuator =	new HumidActuator();
	_pHumidActuator->init();

	string host("127.0.0.1");
	_pDC = new DisplayClient( host, 48557 ); 
	//_pDC->init(); // there's no init() in DisplayClient

  _pSLC = new SessionLoggerClient( host, 48558 );

	// Instantiate Signal 
	_pSig = new Signal();

	// Instantiate TempAlgo
	_pTempAlgo = new TempAlgo();

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
	if( _pHeatFlowActuator ) {
		_pHeatFlowActuator->deinit();	
		delete _pHeatFlowActuator;
		clog << "_pHeatFlowActuator is deinitialized.\n";
	}
	if( _pRollerActuator ) {
		_pRollerActuator->deinit();	
		delete _pRollerActuator;
		clog << "_pRollerActuator is deinitialized.\n";
	}
	if( _pHumidActuator ) {
		_pHumidActuator->deinit();	
		delete _pHumidActuator;
		clog << "_pHumidActuator is deinitialized.\n";
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

	if( _pSLC ) {
		delete _pSLC;
		clog << "_pSLC is deinitialized.\n";
	}

	if( _pSig ) {
		//delete _pSig; // singleton shall be destroyed at program exit.
		clog << "_pSig is deinitialized.\n";
	}

	if( _pTempAlgo ) {
		delete _pTempAlgo;
		clog << "_pTempAlgo is deinitialized.\n";
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
	static int tempSensorFailureCount = 0; 

	if( !_initialized )
	 return;

	// get the control formula from env
	formula_t f;
	unsigned daysPassed = _pSTime->daysPassed();
	
	if( _pEnv->getFormula( daysPassed, f ) ) {
	 cerr << "Can't get formula.\n";
	 return;
	} 
	
	// temperature control

	float tm;
	bool airFlowOverridden4TempControl = false;
	if( !_pTempSensor->get( tm ) ) {

		tempSensorFailureCount = 0;

		clog << "real-time temperature: " << tm << " oC" << '\n';

		// high temperature control
		if( tm >= f.tempHigherLimit + 0.1 ) { // temperature too high
			_pAirFlowActuator->start( LEVEL_ON );	
			//_pHeatFlowActuator->on();	
			airFlowOverridden4TempControl = true;
			clog << "airflow actuator ON and airflow is overridden." << '\n';
		} else {
			//_pAirFlowActuator->stop();	
			airFlowOverridden4TempControl = false;
			//clog << "airflow level: " << f.airFlowLevel << " and airflow is to be normal ." << '\n';
		}

		// normal temperature control
		///////// note that this part is rewritten to behavior like pi controller
		_pTempAlgo->tryGoOnRec( _pSTime->getElapsed(), tm );
		pair<float,float> tempPair = _pTempAlgo->getPair( f.tempLowerLimit, f.tempHigherLimit );
		float tempLowerLimit = tempPair.first;
		float tempHigherLimit = tempPair.second;

		if( tm > tempHigherLimit ) {
			_pHeatActuator->off();
			clog << "heat actuator OFF." << '\n';
			_pHeatFlowActuator->on();
			clog << "heatflow actuator ON." << '\n';
		}
		else if( tm > ( tempHigherLimit + tempLowerLimit ) / 2. ) {
			_pHeatFlowActuator->on();
			clog << "heatflow actuator ON." << '\n';
		}
		else if( tm > tempLowerLimit && tm < ( tempHigherLimit + tempLowerLimit ) / 2. ) {
			if( _pHeatActuator->get() != LEVEL_ON ) {
				_pHeatFlowActuator->off();
				clog << "heatflow actuator OFF." << '\n';
			}
		}
		else if( tm < tempLowerLimit ) {
			_pHeatActuator->on();
			clog << "heat actuator ON." << '\n';
			_pHeatFlowActuator->on();
			clog << "heatflow actuator ON." << '\n';
		}
		/////////

	} else { // can't get the value from temperature sensor
		++tempSensorFailureCount;
		if( tempSensorFailureCount >= 3 ) { // consecutive failures for more than three times
			_pHeatActuator->off(); // prevent over-heating
		}
	}

	// humidity control

	float th;
	if( !_pHumidSensor->get( th ) ) {
		clog << "real-time humidity: " << th << " %" << '\n';

		// dehumidfier control 
		if( th >= f.humidHigherLimit && tm > (f.tempLowerLimit + f.tempHigherLimit ) / 2.0 ) {
			_pDehumidActuator->start( LEVEL_ON );
			clog << "dehumid actuator ON." << '\n';
		}
		else {
			_pDehumidActuator->off();
			clog << "dehumid actuator OFF." << '\n';
		}

		// humidifier control
		if( th < f.humidLowerLimit ) {
			_pHumidActuator->start( LEVEL_50 );
			clog << "humid actuator LEVEL_50." << '\n';
			//_pHumidActuator->on();
		}
		else if( th > ( f.humidLowerLimit + f.humidHigherLimit ) / 2.0 ) {
			_pHumidActuator->stop();
			//_pHumidActuator->off();
			clog << "humid actuator OFF." << '\n';
		}
	}

	// air flow control
	if( !airFlowOverridden4TempControl ) {
		if( tm > ( (f.tempLowerLimit + f.tempHigherLimit ) / 2.0 ) ) {
			_pAirFlowActuator->start( f.airFlowLevel );
			clog << "airflow actuator level: " << f.airFlowLevel << '\n';
		}
		else {
			_pAirFlowActuator->stop();
			clog << "airflow actuator STOP." << '\n';
		}

		// Comment out! as airflow fan is now outside of the incubator
		//if( _pHeatActuator->get() == LEVEL_ON ) {
		//	_pAirFlowActuator->start( LEVEL_50 );
		//} else {
		//	_pAirFlowActuator->start( f.airFlowLevel );
		//}
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
			//this_thread::sleep_for( std::chrono::milliseconds(6000) );
			this_thread::sleep_for( std::chrono::milliseconds(3000) );
			_pRollerActuator->off();	
			clog << "roller actuator ends." << '\n';
			time( &stamp );
		}	
	}

	return;
}

// show stats on lcd by sending message to display server
void Incubator::updatePanel() const {

	// for panel header
	unsigned daysPassed = _pSTime->daysPassed();
	formula_t f;
	if( _pEnv->getFormula( daysPassed, f ) )
		return;

	string header("");
	header += "Days: "; 
	header += to_string( daysPassed );

	// for info1 - temperature 
	string info1("");
	float temp;
	if( _pTempSensor->getCache( temp ) )
		temp = 100.00;
	ostringstream oss1;
	oss1.precision( 2 );
	oss1 << std::fixed << temp << " oC";
	info1 += oss1.str();
	oss1.clear();

	// for info2 - humidity 
	string info2("");
	float humid;
	if( _pHumidSensor->getCache( humid ) )
		humid = 100.00;
	ostringstream oss2;
	oss2.precision( 2 );
	oss2 << std::fixed << humid << " %";
	info2 += oss2.str();
	oss2.clear();
 
	// for info3 - reserved
	string info3("");

	// for footer
	string footer("");
	time_t now; time(&now);
	struct tm *timeptr;
	timeptr = localtime(&now);
	char s[100];
	strftime( s, sizeof(s), "%a %b %d %R", timeptr );
	footer += s;
	
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
	msg += '\0';
	_pDC->sendMsg( msg ); 
	clog << "Incubator: pDC->sendMsg(): " << msg << endl;

	return;
}

// show stats on session log by sending message to log server
void Incubator::updateSessionLog() const {

	string dt, msg; // date/time, message

	//dt = IncUtil::dateTime2Str();
	dt = dateTime2Str();

  float temp;
  if( _pTempSensor->getCache( temp ) )
    temp = 100.00;
	
	float humid;
  if( _pHumidSensor->getCache( humid ) )
    humid = 100.00;

	// format string, strFormat() does not work using gcc on rpi although it worked on my macbook.
	//msg = IncUtil::strFormat( "%10d %20s %8.2f %8.2f %5d", _runCount, dt.c_str(), temp, humid, _pRollerActuator->getCount() );	
	//msg = strFormat( "%10d %20s %8.2f %8.2f %5d", _runCount, dt.c_str(), temp, humid, _pRollerActuator->getCount() );	
	stringstream ss;
	ss << setfill(' ') << setw(10) << _runCount 
		<< setfill(' ') << setw(20) << dt.c_str() 
		<< setfill(' ') << setw(8) << setprecision(2) << temp
		<< setfill(' ') << setw(8) << setprecision(2) << humid
		<< setfill(' ') << setw(5) << _pRollerActuator->getCount();
	msg = ss.str();

	// send string to session logger 
	_pSLC->sendMsg( msg );
	
	return;
}

// loop for incubator control
void Incubator::runLoop() {
	static time_t sessionLogStamp=0;
	_runCount =  0;

	if( !_initialized )
		return;

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
		if( daysPassed < maxDay ) { // active session is on-going
			_runCount++;
			clog << "\n[" << _runCount << "] " << _pSTime->daysPassed() << " days passed or " << _pSTime->getElapsed() << " ticks elapsed.\n";

			_run();
			_run4Roller();

			updatePanel();

			// update session log once in every 1 minute.
			time_t now;
			time( &now );
			if( now - sessionLogStamp >= 60 ) {
				updateSessionLog();
				time( &sessionLogStamp );	
			}
		}
		else { // session has been completed.
			clog << "Max day passed: " << "maxDay: " << maxDay << ", " << "daysPassed: " << daysPassed << endl;
			clog << "All actuators is off if not." << endl;

			if( _pAirFlowActuator->get() != LEVEL_OFF ) 
				_pAirFlowActuator->off();	
			if( _pDehumidActuator->get() != LEVEL_OFF ) 
				_pDehumidActuator->off();
			if( _pHeatActuator->get() != LEVEL_OFF ) 
				_pHeatActuator->off();
			if( _pHeatFlowActuator->get() != LEVEL_OFF ) 
				_pHeatFlowActuator->off();
			if( _pRollerActuator->get() != LEVEL_OFF ) 
				_pRollerActuator->off();
			if( _pHumidActuator->get() != LEVEL_OFF ) 
				_pHumidActuator->off();

			//this_thread::sleep_for( std::chrono::milliseconds(1000) );
			//return;
		}

		// some sensors has a limitation on the consecutive reading. dht22 allows to read next at least after two seconds later.
		this_thread::sleep_for( std::chrono::milliseconds(3000) );
		
		// notify systemd watchdog only if systemd expects notification.
//#ifdef DO_SD_NOTIFY
		uint64_t usec;
		if( sd_watchdog_enabled( 0, &usec ) > 0 ) {
			sd_notify (0, "WATCHDOG=1");
			//clog << "sd_notify().\n";
		} 
//#endif
	}

	// will get here only by SIGTERM
	Signal::resetSignalTerm(); // needed?
	clog << "Incubator got SIGTERM and is shutting down.\n";

	return;
}

int main( int argc, char *argv[] ) {

	if( argc != 2 ) {
		cerr << "Usage: egg-incubator param!\n";
		cerr << "Note that param can either 0 for continuing existing session, or 1 for initiating a new session.\n"; 
		return -1;
	}
 
	// create singleton incubator instance
	Incubator& inc  = Incubator::getInstance();

	// all composited components are initialized.
	inc.init();

	if( !strcmp( argv[1], "1" ) ) {
		inc.newSession();
	} 

//#ifdef DO_SD_NOTIFY
	uint64_t usec;
	if( sd_watchdog_enabled( 0, &usec ) > 0 ) {
		sd_notify (0, "READY=1");
		clog << "systemd watchdog is enabled. will send notify.\n";	
	} 
//#endif

	inc.runLoop();
	inc.deinit();

	return 0;
}

// EOF

