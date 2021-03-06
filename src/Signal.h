// Signal.h

#ifndef __SIGNAL_H__
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
  static std::atomic<bool> atomicSigTerm_;
  static volatile sig_atomic_t atomicSigUsr1;
  static std::atomic<bool> atomicSigUsr1_;

	// signal handler
  static void signalHandler( int signum );

	// helper
	static bool isSignaledTerm();
	static bool isSignaledUsr1();	
  static void resetSignalTerm();
  static void resetSignalUsr1();

	Signal();
	virtual ~Signal();
};

#endif
// EOF

