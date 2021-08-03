// Actuator.h

#ifndef __ACTUATOR_H__
#define __ACTUATOR_H__

enum ActStatus {
	HIGH,
	MID,
	LOW,
	OFF	
}

class Actuator {
public:
	virtual ~Actuator() {}
	virtual void set( ActStatus status ) = 0;
	virtual ActStatus get() = 0;
	virtual void start() = 0;
	virtual void stop() = 0;
}

#endif

