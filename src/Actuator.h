// Actuator.h

#ifndef __ACTUATOR_H__
#define __ACTUATOR_H__

#include <iostream>

#define LEVEL_ON 100
#define LEVEL_50 50 
#define LEVEL_OFF 0
#define LEVEL_INVALID 0xffffffff 
typedef unsigned level_t;

struct Actuator {
	virtual ~Actuator() {}
	virtual void init() = 0;
	virtual void deinit() = 0;
	//virtual level_t get() = 0;
	virtual void on() = 0;
	virtual void off() = 0;
	virtual void start( level_t ) { std::clog << "start(level) is not implemented in the derived classs.\n"; }
	virtual void stop() { std::clog << "stop() is not implemented in the derived class. \n"; }
};

#endif

