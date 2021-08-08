// Signal.cpp

#include <iostream>
#include "Signal.h"

volatile sig_atomic_t Signal::atomicSigTerm{0};
std::atomic<bool> Signal::atomicSigTerm_{false};
volatile sig_atomic_t Signal::atomicSigUsr1{0};
std::atomic<bool> Signal::atomicSigUsr1_{false};

bool Signal::isSignaledTerm() {
	if( Signal::atomicSigTerm || Signal::atomicSigTerm_.load() ) {
		std::clog << "Got SIGTERM.\n";
		return true;
	}
	return false;
}

bool Signal::isSignaledUsr1() {
	if( Signal::atomicSigUsr1 || Signal::atomicSigUsr1_.load() ) {
		std::clog << "Got SIGUSR1.\n";
		return true;
	}
	return false;
}

// my signal handler to break from monitoring loop
void Signal::signalHandler( int signum ) {
  if( signum == SIGTERM ) {
		Signal::atomicSigTerm = 1;
		Signal::atomicSigTerm_ = true;
  	const char str[] = "SIGTERM received!\n";
  	write( STDERR_FILENO, str, sizeof(str)-1 ); // write is signal safe!
  }
  if( signum == SIGUSR1 ) {
		Signal::atomicSigUsr1 = 1;
		Signal::atomicSigUsr1_ = true;
  	const char str[] = "SIGUSR1 received!\n";
  	write( STDERR_FILENO, str, sizeof(str)-1 ); // write is signal safe!
  }
	return;
  //exit( signum );
}

Signal::Signal() {
  struct sigaction action;
  action.sa_handler = Signal::signalHandler;
  sigemptyset( &action.sa_mask );
  action.sa_flags = 0;
  sigaction( SIGTERM, &action, NULL );
  sigaction( SIGUSR1, &action, NULL );

  Signal::atomicSigTerm = 0;
  Signal::atomicSigTerm_ = false; //std::atomic is safe, as long as it's lock-free
  assert( atomicSigTerm_.is_lock_free() );

  Signal::atomicSigUsr1 = 0;
  Signal::atomicSigUsr1_ = false; //std::atomic is safe, as long as it's lock-free
  assert( atomicSigUsr1_.is_lock_free() );

	return;
}

Signal::~Signal() {
	struct sigaction action;
	action.sa_handler = SIG_DFL; // restore default handler 
  sigemptyset( &action.sa_mask );
  action.sa_flags = 0;
  sigaction( SIGTERM, &action, NULL );
  sigaction( SIGUSR1, &action, NULL );

	return;
}

// EOF

