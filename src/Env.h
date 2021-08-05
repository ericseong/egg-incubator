// Env.h
//
// Env is singleton. Get the instance as below.
// Env& env = Env::getInstance();

#ifndef __ENV_H__
#define __ENV_H__

#include <jsoncpp/json/json.h>
#include <string>
#include "Singleton.h"

using namespace std;

#define NUM_PHASES 4

typedef struct _phase {
	string name;
	unsigned startDate;
	unsigned endDate;
	float tempLowerLimit;
	float tempHigherLimit;
	float humidLowerLimit;
	float humidHigherLimit;
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
	bool _initialized;
	string _text;
	config_t _config;
	int	_readConfig( const string& cfgFile );
	int _readFromCfgFile( const string& cfgFile );
	void _str2Obj( const string& str, Json::Value& obj ) const;
	void _objByPhaseName( const Json::Value& phases, const string& phaseName, Json::Value& objPhase ) const;
	void _obj2Phase( const Json::Value& objPhase, phase_t& ph ) const;
	void _obj2Config( const Json::Value& session, config_t& cfg ) const;
public:
	Env() : _initialized(false) {}
	virtual ~Env() {}
	int setUp( const string& cfgFileName );
	int getConfig( config_t& cfg ) const;
	int setConfig( const string cfgFileName ) const;
};

#endif

// EOF

