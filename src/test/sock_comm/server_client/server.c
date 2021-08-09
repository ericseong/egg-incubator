// server.c
// Original code from the paper "A guide to inter process communication in Linux" by Marty Kalin.

#include <string.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <errno.h> 
#include <fcntl.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/tcp.h> 
#include <arpa/inet.h> 
//#include "sock.h"

const unsigned PortNumber = 48557;
const unsigned MaxConnects = 10;
const unsigned BuffSize = 1024;

int guard(int n, char * err) { if (n == -1) { perror(err); exit(1); } return n; }

int main() {
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
	saddr.sin_port = htons(PortNumber); /* for listening */

	guard (bind(fd, (struct sockaddr *) &saddr, sizeof(saddr)), "could not bind" );

	/* listen to the socket */
	guard( listen(fd, MaxConnects), "could not listen" ); /* listen for clients, up to MaxConnects */

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
		char buffer[BuffSize + 1];
		memset(buffer, '\0', sizeof(buffer));
		int count = read(client_fd, buffer, sizeof(buffer)); 
		if (count > 0) {
			puts(buffer);
			write(client_fd, buffer, sizeof(buffer)); /* echo as confirmation */ 
		} else { // will be here for O_NONBLOCK
		}

		close(client_fd); /* break connection */ 
	} /* while(1) */

	return 0;
}

//  eof

