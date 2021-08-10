// DisplayServer.cpp
//
// Message format:
// text:color:text:color:text:color:text:color
// daysPassed, Temperature, Humidity, Last updated

#include <string>
#include "DisplayServer.h"

// refresh display with the string gotten from Incubator process
void DisplayServer::_updateDisplay( char *msg ) const {
		
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
			_updateDisplay( buffer );	
#if 1 // for test
			puts(buffer);
			write(client_fd, buffer, sizeof(buffer)); /* echo as confirmation */ 
#endif
		} else { // can be here for O_NONBLOCK
			;
		}

		close(client_fd); /* break connection */ 
	} /* while(1) */

	return;
}

#if 0 // for test
int main() {

	DisplayServer ds( 48557, 10 ); // port no. and max connets
	ds.run();

	return 0;
}
#endif

// EOF

