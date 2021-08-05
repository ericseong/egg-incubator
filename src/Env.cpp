// Env.cpp

#include <iostream>
#include <fstream>
#include "Env.h"

const string cfgFileName( "config.json" );
const string phaseNames[NUM_PHASES] = {
	"warming-up",
	"incubating",
	"hatching",
	"feeding"	
};

// read cfg file into text string
int Env::_readCfgFile( string& text ) const {

  ifstream ifs( cfgFileName.c_str() );
  stringstream buf;

  buf << ifs.rdbuf();
	ifs.close();

  text = buf.str();
  clog << text << endl;
	
	return 0;
}

// convert string to json object
void Env::_str2Obj( const string& str, Json::Value& obj ) const {
	Json::CharReaderBuilder rb;
	Json::CharReader *reader = rb.newCharReader();
	string errors;
  bool success = reader->parse(str.c_str(), str.c_str() + str.size(), &obj, &errors);
  delete reader;

	if( !success ) {
		cerr << errors << endl;
	}

	return;
}	

// get phase object with given name. output will be stored in objPhase
void Env::_objByPhaseName( const Json::Value& phases, const string& phaseName, Json::Value& objPhase ) const {
  for( const Json::Value& phase : phases ) {
    if( phase["name"].asString() == phaseName ) {
      clog << phaseName << " is found!" << endl;
      objPhase = phase;
    }
  }

  return;
}

// convert phase (json obj) into phase (structure)
void Env::_obj2Phase( const Json::Value& objPhase, phase_t& ph ) const {

	ph.name = objPhase["name"].asString();
	ph.startDate = objPhase["s1"].asUInt();
	ph.endDate = objPhase["s2"].asUInt();
	ph.tempLowerLimit = objPhase["t1"].asFloat();
	ph.tempHigherLimit = objPhase["t2"].asFloat();
	ph.humidLowerLimit = objPhase["h1"].asFloat();
	ph.humidHigherLimit = objPhase["h2"].asFloat();
	ph.airFlowLevel = objPhase["af"].asUInt();
	ph.rollInterval = objPhase["ro"].asUInt();

	return;
}

// convert the whole session config to config_t structure
void Env::_obj2Config( const Json::Value& session, config_t& cfg ) const {
	cfg.name = session["name"].asString();		
	cfg.days = session["days"].asUInt();
	cfg.maxDays = session["max_days"].asUInt();
	
	Json::Value phases = session["phases"];
	Json::Value obj;
	for( int i = 0 ; i < NUM_PHASES; ++i ) {
		_objByPhaseName( phases, phaseNames[i], obj );
		_obj2Phase( obj, cfg.phases[i] );
	}

	return;
}

int Env::getConfig( config_t& cfg ) {
	string text;

	// read config file to a string
	if( _readCfgFile( text ) ) {
		cerr << "Can't read from cfg file." << endl;
		cerr << strerror( errno ) << endl;	
		return -1;
	}

	// convert string to json obj
	Json::Value obj;
	_str2Obj( text, obj ); 

	// get config structure from the json obj
	_obj2Config( obj["session"], cfg );

	return 0;
}

// TODO!
int Env::setConfig( config_t& cfg ) {
	return 0;
}

// EOF

