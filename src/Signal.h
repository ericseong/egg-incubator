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
	// atomic variables for comm. between signal handler and a process
  static volatile sig_atomic_t atomicSigTerm;
  static atomic<bool> atomicSigTerm_;
  static volatile sig_atomic_t atomicSigUsr1;
  static atomic<bool> atomicSigUsr1_;

	// signal handler
  static void signalHandler( int signum );

	// helper
	static bool isSignaledTerm();
	static bool isSignaledUsr1();	

	Signal();
	virtual ~Signal();
};

#endif
// EOF

