// Env.h
//
// Env is singleton. Get the instance as below.
// Env& env = Env::getInstance();

#ifndef __ENV_H__
#define __ENV_H__

#include <jsoncpp/json/json.h>
#include <string>
#include <vector>
#include "Singleton.h"

#define NUM_PHASES 4

typedef struct _phase_header {
	string name;
	unsigned startDate;
	unsigned endDate;
} phase_header_t;

typedef struct _fomula {
	float tempLowerLimit;
	float tempHigherLimit;
	float humidLowerLimit;
	float humidHigherLimit;
	unsigned airFlowLevel;
	unsigned rollInterval;
} formula_t;

typedef struct _phase {
	phase_header_t header;
	formula_t body;
} phase_t;

typedef struct _config {
	string name;
	unsigned days;
	phase_t phases[NUM_PHASES];
} config_t;

// vector index = which day, map (key, value) 
typedef struct formulas {
	vector<formula_t> formulas;
} formulas_t; 

class Env : public Singleton<Env> {
	bool _initialized;
	string _text;
	config_t _config;
	formulas_t _formulas;
	int	_readConfig( const string& cfgFile );
	int _readFromCfgFile( const string& cfgFile );
	void _str2Obj( const string& str, Json::Value& obj ) const;
	void _objByPhaseName( const Json::Value& phases, const string& phaseName, Json::Value& objPhase ) const;
	void _obj2Phase( const Json::Value& objPhase, phase_t& ph ) const;
	void _obj2Config( const Json::Value& session, config_t& cfg ) const;
	void _cfg2Formulas( config_t& cfg, formulas_t& forms ) const;
public:
	Env() : _initialized(false) {}
	virtual ~Env() {}
	int setUp( const string& cfgFileName );
	int getConfig( config_t& cfg ) const;
	int setConfig( const string cfgFileName ) const;
	int getFormula( unsigned day, formula_t& formula ) const;   
};

#endif

// EOF

