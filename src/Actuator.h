// Actuator.h

#ifndef __ACTUATOR_H__
#define __ACTUATOR_H__

#define LEVEL_ON 100
#define LEVEL_50 50 
#define LEVEL_OFF 0
#define LEVEL_INVALID 0xffffffff 
typedef unsigned level_t;

class Actuator {
public:
	virtual ~Actuator() {}
	virtual void init() = 0;
	virtual void deinit() = 0;
	virtual level_t get() = 0;
	virtual void start() = 0;
	virtual void stop() = 0;
};

#endif

