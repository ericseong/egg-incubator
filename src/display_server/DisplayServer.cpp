// DisplayServer.cpp
//
// Message format:
// text:color:text:color:text:color:text:color
//
// Contents are: daysPassed, Temperature, Humidity, reserved, Updated or,
// header, info1, info2, info3, footer

#include <systemd/sd-daemon.h>
#include <stdint.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <iostream>
#include "DisplayServer.h"
#include "InfoPanel.h"

using namespace std;

static int guard(int n, char * err) { if (n == -1) { perror(err); exit(1); } return n; }

// from https://stackoverflow.com/questions/236129/how-do-i-iterate-over-the-words-of-a-string?rq=1
template <typename Out> void split(const std::string &s, char delim, Out result) {
	std::istringstream iss(s);
	std::string item;
	while (std::getline(iss, item, delim)) {
		*result++ = item;
	}
}
std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, std::back_inserter(elems));
	return elems;
}
// end of from

// refresh display with the string gotten from Incubator process
void DisplayServer::_updateDisplay( char *msg ) const {

	string smsg(msg);
	std::vector<std::string> x = split(smsg, '$');
	if( x.size() != 10 ) {
		clog << "In display server, received string is with unexpected size " << x.size() << endl;
		return;
	}

	uint16_t headerColor =	( x[1] == "white" ? WHITE : x[1] == "red" ? RED : x[1] == "blue" ? BLUE : WHITE ); 
	uint16_t info1Color =	( x[3] == "white" ? WHITE : x[3] == "red" ? RED : x[3] == "blue" ? BLUE : WHITE ); 
	uint16_t info2Color =	( x[5] == "white" ? WHITE : x[5] == "red" ? RED : x[5] == "blue" ? BLUE : WHITE ); 
	uint16_t info3Color =	( x[7] == "white" ? WHITE : x[7] == "red" ? RED : x[7] == "blue" ? BLUE : WHITE ); 
	uint16_t footerColor =	( x[9] == "white" ? WHITE : x[9] == "red" ? RED : x[9] == "blue" ? BLUE : WHITE ); 

	_pIP->drawInfoPanel( 
			x[0], headerColor,
			x[2], info1Color,
			x[4], info2Color,
			x[6], info3Color,
			x[8], footerColor
	);	
	
	return;
}

void DisplayServer::run() {

	int fd = guard( socket( 
		AF_INET, /* network versus AF_LOCAL */ 
		SOCK_STREAM, /* reliable, bidirectional, arbitrary payload size */ 
		0), "could not create TCP listening socket" 
	); /* system picks underlying protocol (TCP) */ 

	int flags = guard( fcntl( fd, F_GETFL ), "could not get flags on TCP listening socket" );
	guard( fcntl( fd, F_SETFL, flags | O_NONBLOCK ), "could not set TCP listening socket to be non-blocking" );

	/* bind the server's local address in memory */
	struct sockaddr_in saddr;
	memset(&saddr, 0, sizeof(saddr)); /* clear the bytes */ 
	saddr.sin_family = AF_INET; /* versus AF_LOCAL */ 
	saddr.sin_addr.s_addr = htonl(INADDR_ANY); /* host-to-network endian */ 
	saddr.sin_port = htons(_portNo); /* for listening */

	guard (bind(fd, (struct sockaddr *) &saddr, sizeof(saddr)), "could not bind" );

	/* listen to the socket */
	guard( listen(fd, _maxConnects), "could not listen" ); /* listen for clients, up to _maxConnects */

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
			_updateDisplay( buffer );	
		} else { // can be here for O_NONBLOCK
			;
		}

		close(client_fd); /* break connection */ 

		// Check if user requests new session by pressing both btn1 and 2
		if( _pIP->isRequestNewSession() ) {
			system("/home/pi/hobby/egg-incubator/src/display_server/newsession.sh");
		}

    // notify systemd watchdog only if systemd expects notification.
//#ifdef DO_SD_NOTIFY
		uint64_t usec;
		if( sd_watchdog_enabled( 0, &usec ) > 0 ) {
			sd_notify (0, "WATCHDOG=1");
			//clog << "sd_notify().\n";
		}
//#endif
		
	} /* while(1) */

	return;
}

int main() {

	DisplayServer ds( 48557, 10 ); // port no. and max connets

//#ifdef DO_SD_NOTIFY
  uint64_t usec;
  if( sd_watchdog_enabled( 0, &usec ) > 0 ) {
    sd_notify (0, "READY=1");
    clog << "systemd watchdog is enabled. will send notify.\n";
  }
//#endif

	ds.run();

	return 0;
}

// EOF

