// Signal.h

#ifdef __SIGNAL_H__
#define __SIGNAL_H__

#include <atomic>
#include <chrono>
#include <thread>
#include <cassert>
#include <signal.h>
#include <unistd.h>
#include "Singleton.h"

class Signal {
public:
  static volatile sig_atomic_t doBreak;
  static atomic<bool> breakRequested;
  static void signalHandler( int signum );
	Signal();
	virtual ~Signal();
};

#endif
// EOF

