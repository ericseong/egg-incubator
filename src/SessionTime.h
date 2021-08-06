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
	inline bool _fileExist( const std::string& fname ) const;
	inline void _createFile( const std::string& fname ) const;
public:
	SessionTime() : _initialized(false), _start(0) {}
	virtual ~SessionTime() {}
	int init();
	int deinit();
	inline time_t getStart() const;
	void setStart();
	inline time_t getCurrent() const;
	inline time_t getElapsed() const;
	inline time_t getLast() const;
	inline void setLast() const;
};

#endif

// EOF

