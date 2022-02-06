// SessionLogger.cpp
//
// Message format:
// Just a plain text! 
//
// Contents are: date/time temp humidity roller_count 

#include <systemd/sd-daemon.h>
#include <sys/stat.h>
#include <stdint.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <vector>
#include <iterator>
#include <iostream>
#include "SessionLogger.h"

bool SessionLogger::_fileExist( const string& fname ) const {
  struct stat buf;
  return ( stat( fname.c_str(), &buf ) == 0 );
}

void SessionLogger::_createFile( const string& fname ) const {
  ofstream ofs(fname);
  ofs << "date/time temperature humidity roller_count" << endl;
  ofs << "--------------------------------------------------" << endl;
  ofs.close();

  return;
}

// log the message into the session log file and its naming follows the content of "session_start.txt".
void SessionLogger::_log( char* buf ) const {

	// see if session is defined
	if( !_fileExist( _sBeginTimeFileName ) ) {
		std::cerr << _sBeginTimeFileName << " does not exist! can't log!" << endl;
		return;
	}

	// get session_start file
  string str;
  ifstream ifs( _sBeginTimeFileName );
  //getline( ifs, str, '\0' );
  getline( ifs, str );
  ifs.close();
	
	// see if log file already exists.
	string session_log = str + ".log";
	if( !_fileExist( session_log ) ) { // new session. create a new empty file.
		_createFile( session_log );
	}

	// append buf to session_log file
	ofstream afs;
	afs.open( session_log, fstream::out | fstream::app ); // open in append mode
	afs << (string)buf << endl;	
	afs.close();

	return;
}

static int guard(int n, char * err) { if (n == -1) { perror(err); exit(1); } return n; }

void SessionLogger::run() {

	int fd = guard( socket( 
		AF_INET, /* network versus AF_LOCAL */ 
		SOCK_STREAM, /* reliable, bidirectional, arbitrary payload size */ 
		0), (char*)"could not create TCP listening socket" 
	); /* system picks underlying protocol (TCP) */ 

	int flags = guard( fcntl( fd, F_GETFL ), (char*)"could not get flags on TCP listening socket" );
	guard( fcntl( fd, F_SETFL, flags | O_NONBLOCK ), (char*)"could not set TCP listening socket to be non-blocking" );

	/* bind the server's local address in memory */
	struct sockaddr_in saddr;
	memset(&saddr, 0, sizeof(saddr)); /* clear the bytes */ 
	saddr.sin_family = AF_INET; /* versus AF_LOCAL */ 
	saddr.sin_addr.s_addr = htonl(INADDR_ANY); /* host-to-network endian */ 
	saddr.sin_port = htons(_portNo); /* for listening */

	guard( ::bind( fd, (struct sockaddr *) &saddr, sizeof(saddr) ), (char*)"could not bind" );

	/* listen to the socket */
	guard( listen( fd, _maxConnects ), (char*)"could not listen" ); /* listen for clients, up to _maxConnects */

	while (1) {
		struct sockaddr_in caddr; /* client address */
		unsigned len = sizeof(caddr); /* address length could change */

		int client_fd = accept(fd, (struct sockaddr*) &caddr, &len); /* accept blocks */ 
		if ( client_fd == -1 ) {
			if( errno == EWOULDBLOCK ) { // will be here for O_NONBLOCK
				//fprintf( stdout, "No pending connection; sleeping for one second.\n" );
				sleep(1);
			} else {
				perror("error when accepting connection");
				continue; // continue anyway!
				//exit(1);
			}
		}

		/* read from client */
		char buffer[_bufSize + 1];
		memset(buffer, '\0', sizeof(buffer));
		int count = read(client_fd, buffer, sizeof(buffer)); 
		if (count > 0) {
			fprintf( stdout, "Read from client: %s\n", buffer );
			_log( buffer );
		} else { // can be here for O_NONBLOCK
			;
		}

		close(client_fd); /* break connection */ 

    notify systemd watchdog only if systemd expects notification.
		uint64_t usec;
		if( sd_watchdog_enabled( 0, &usec ) > 0 ) {
			sd_notify (0, "WATCHDOG=1");
			//clog << "sd_notify().\n";
		}
		
	} /* while(1) */

	return;
}

int main() {

	SessionLogger sl( 48558, 10, "session_start.txt" ); 
	// port no., max connets, session start time file name, session log file name. 
	// note that session start time file name should be the same as the one defined in SessionTime.cpp."

  uint64_t usec;
  if( sd_watchdog_enabled( 0, &usec ) > 0 ) {
   sd_notify (0, "READY=1");
   clog << "systemd watchdog is enabled. will send notify.\n";
  }

	sl.run();

	return 0;
}

// EOF

