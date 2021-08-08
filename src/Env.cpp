// Env.cpp

#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Env.h"

using namespace std;

const string phaseNames[NUM_PHASES] = {
	"warming-up",
	"incubating",
	"hatching",
	"feeding"	
};

// read cfg file into text string
int Env::_readFromCfgFile( const string& cfgFileName ) {

  ifstream ifs( cfgFileName.c_str() );
  stringstream buf;

  buf << ifs.rdbuf();
	ifs.close();

  _text = buf.str();
  clog << _text << endl;
	
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

	ph.header.name = objPhase["name"].asString();
	ph.header.startDate = objPhase["s1"].asUInt();
	ph.header.endDate = objPhase["s2"].asUInt();
	ph.body.tempLowerLimit = objPhase["t1"].asFloat();
	ph.body.tempHigherLimit = objPhase["t2"].asFloat();
	ph.body.humidLowerLimit = objPhase["h1"].asFloat();
	ph.body.humidHigherLimit = objPhase["h2"].asFloat();
	ph.body.airFlowLevel = objPhase["af"].asUInt();
	ph.body.rollInterval = objPhase["ro"].asUInt();

	return;
}

// convert the whole session config to config_t structure
void Env::_obj2Config( const Json::Value& session, config_t& cfg ) const {
	cfg.name = session["name"].asString();		
	cfg.days = session["days"].asUInt();
	
	Json::Value phases = session["phases"];
	Json::Value obj;
	for( int i = 0 ; i < NUM_PHASES; ++i ) {
		_objByPhaseName( phases, phaseNames[i], obj );
		_obj2Phase( obj, cfg.phases[i] );
	}

	return;
}

// convert cfg to forms of vector<formula_t> 
void Env::_cfg2Formulas( config_t& cfg, std::vector<formula_t>& forms ) const {
	bool found;
	unsigned day = 0;

	while( day < cfg.days ) {
		found = false;
		int p;
		for( p =  0 ; p < NUM_PHASES ; p++ ) {
			if( cfg.phases[p].header.startDate == day ) {
				found = true;
				break; // found	
			}
		}
		if( !found ) { // something wrong!
			cerr << "_cfg2Formulas(): Can't find phases.\n";
			return;
		}
		while( day <= cfg.phases[p].header.endDate ) {
			forms.push_back( cfg.phases[p].body );
			++day;
		}
	}
	return;
}	

// Read from config file, parse json objects and store to _config 
int Env::_readConfig( const string& cfgFile ) {

	// read config file to a string
	if( _readFromCfgFile( cfgFile ) ) {
		cerr << "Can't read from cfg file." << endl;
		cerr << strerror( errno ) << endl;	
		return -1;
	}

	// convert string to json obj
	Json::Value obj;
	_str2Obj( _text, obj ); 

	// get config structure from the json obj
	_obj2Config( obj["session"], _config );

	// get formula per day
	_cfg2Formulas( _config, _formulas );
	
	// for debug
	clog << "_formulas: \n";
	for( unsigned d = 0 ; d < _config.days ; d++ ) {
		clog << "day " << d << ": ";
		clog << "TL: " << _formulas[d].tempLowerLimit << ", "; 
		clog << "TH: " << _formulas[d].tempHigherLimit << ", "; 
		clog << "HL: " << _formulas[d].humidLowerLimit << ", "; 
		clog << "HH: " << _formulas[d].humidHigherLimit << ", "; 
		clog << "AF: " << _formulas[d].airFlowLevel << ", "; 
		clog << "RI: " << _formulas[d].rollInterval << endl; 
	}

	return 0;
}

// Simply get confit_t structure from object variable _config 
int Env::getConfig( config_t& cfg ) const {
	cfg = _config;	
	return 0;
}

// get the formula per day
int Env::getFormula( unsigned day, formula_t& formula ) const {

	formula = _formulas[day];
	return 0;
}

// write _config to back to cfg file
int Env::setConfig( const string cfgFileName ) const {
	// TODO! if needed.
	return 0;
}

// Read from config file, parse json objects and store to _config 
int Env::setUp( const string& cfgFileName ) {
	if( _readConfig( cfgFileName ) ) {
		cerr << "_readConfig() failed.\n";
		return -1;
	}

	return 0;
}

// EOF

