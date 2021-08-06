// Signal.cpp

#include "Signal.h"

// my signal handler to break from monitoring loop
void Signal::signalHandler( int signum ) {

  if( signum == SIGTERM ) {
		Signal::doBreak = 1;
		Signal::breakRequested = true;
  }
  if( signum == SIGUSR1 ) {
    // TODO!!
  }

  const char str[] = "Signal received!\n";
  write( STDERR_FILENO, str, sizeof(str)-1 ); // write is signal safe!

  exit( signum );
}

void Signal::Signal() {
  struct sigaction action;
  action.sa_handler = Incubator::signalHandler;
  sigemptyset( &action.sa_mask );
  action.sa_flags = 0;
  sigaction( SIGTERM, &action, NULL );
  sigaction( SIGUSR1, &action, NULL );

  Signal::doBreak = 0;
  Signal::breakRequested = false; //std::atomic is safe, as long as it's lock-free
  assert( breakRequested.is_lock_free() );
}

Signal::~Signal() {}

// EOF

