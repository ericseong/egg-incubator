// Signal.cpp

#include "Signal.h"

bool Signal::isSignaledTerm() {
	if( Signal::atomicSigTerm || Signal::atomicSigTerm_.load() ) {
		return true;
	}
	return false;
}

bool Signal::isSignaledUsr1() {
	if( Signal::atomicSigUsr1 || Signal::atomicSigUsr1_.load() ) {
		return true;
	}
	return false;
}

// my signal handler to break from monitoring loop
void Signal::signalHandler( int signum ) {

  if( signum == SIGTERM ) {
		Signal::atomicSigTerm = 1;
		Signal::atomicSigTerm_ = true;
  }
  if( signum == SIGUSR1 ) {
		Signal::atomicSigUsr1 = 1;
		Signal::atomicSigUsr1_ = true;
  }

  const char str[] = "Signal received!\n";
  write( STDERR_FILENO, str, sizeof(str)-1 ); // write is signal safe!

  exit( signum );
}

void Signal::Signal() {
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
}

Signal::~Signal() {}

// EOF

