// Time.h
#ifndef __SESSIONTIME_H__
#define __SESSIONTIME_H__

#include <unistd.h>
#include <time.h>
#include <string>
#include "Singleton.h"

class SessionTime : public Singleton<SessionTime> {
	bool _initialized;
	time_t _start;
	bool _fileExist( const std::string& fname ) const;
	void _createFile( const std::string& fname ) const;
public:
	SessionTime() : _initialized(false), _start(0) {}
	virtual ~SessionTime() {}
	int init();
	int deinit();
	time_t getStart() const;
	void setStart();
	time_t getCurrent() const;
	time_t getElapsed() const;
	//time_t getLast() const;
	//void setLast() const;
	unsigned daysPassed() const;
};

#endif

// EOF

