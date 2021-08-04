// Actuator.h

#ifndef __ACTUATOR_H__
#define __ACTUATOR_H__

enum level_t {
	OFF = 0,
	LEVEL_10 = 10,
	LEVEL_20 = 20,
	LEVEL_30 = 30,
	LEVEL_40 = 40,
	LEVEL_50 = 50,
	LEVEL_60 = 60,
	LEVEL_70 = 70,
	LEVEL_80 = 80,
	LEVEL_90 = 90,
	ON = 100,
	INVALID = 0xffffffff
};

class Actuator {
public:
	virtual ~Actuator() {}
	virtual void init() = 0;
	virtual level_t get() = 0;
	virtual void start() = 0;
	virtual void stop() = 0;
};

#endif

