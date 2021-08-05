// Env.h
//
// Env is singleton. Get the instance as below.
// Env& env = Env::getInstance();

#ifndef __ENV_H__
#define __ENV_H__

#include <json/json.h>
#include <string>
#include "Singleton.h"

using namespace std;

#define NUM_PHASES 4

typedef struct _phase {
	string name;
	unsigned startDate;
	unsigned endDate;
	unsigned tempLowerLimit;
	unsigned tempHigherLimit;
	unsigned humidLowerLimit;
	unsigned humidHigherLimit;
	unsigned airFlowLevel;
	unsigned rollInterval;
} phase_t;

typedef struct _config {
	string name;
	unsigned days;
	unsigned maxDays;
	phase_t phases[NUM_PHASES];
} config_t;

class Env : public Singleton<Env> {
	string _text;
	config_t _config;
	int _readCfgFile( string& text ) const;
	void _str2Obj( const string& str, Json::Value& obj ) const;
	void _objByPhaseName( const Json::Value& phases, const string& phaseName, Json::Value& objPhase ) const;
	void _obj2Phase( const Json::Value& objPhase, phase_t& ph ) const;
	void _obj2Config( const Json::Value& session, config_t& cfg ) const;
public:
	virtual ~Env() {}
	int getConfig( config_t& cfg );
	int setConfig( config_t& cfg );
};

#endif

// EOF

