// client.c
// Original code from the paper "A guide to inter process communication in Linux" by Marty Kalin.

#include <string.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <fcntl.h> 
#include <errno.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <netinet/tcp.h> 
#include <netdb.h> 
//#include "sock.h"

const unsigned PortNumber = 48557; // see if the port is not used in the system by looking at /etc/services
const unsigned BuffSize = 1024;
const char Host[] = "127.0.0.1";

const char* message = "What is the youth.. Impetuous fire.. What is a maid.. Ice and desire.. The world wags on..";

int guard(int n, char * err) { if (n == -1) { perror(err); exit(1); } return n; }

int main() {

	/* fd for the socket */
	int fd = guard( socket(AF_INET, /* versus AF_LOCAL */
		SOCK_STREAM, /* reliable, bidirectional */
		0), "could not create TCP listening socket" ); /* system picks protocol (TCP) */ 

	//int flags = guard( fcntl( fd, F_GETFL ), "could not get flags on TCP listening socket" );
	//guard( fcntl( fd, F_SETFL, flags | O_NONBLOCK ), "could not set TCP listening socket to be non-blocking" );
	
	/* get the address of the host */
	struct hostent* hptr = gethostbyname(Host); /* localhost: 127.0.0.1 */ 
	if (!hptr) 
		fprintf( stderr, "gethostbyname failed" ); /* is hptr NULL? */
	if (hptr->h_addrtype != AF_INET) /* versus AF_LOCAL */
		fprintf( stderr, "bad address family" );

	/* connect to the server: configure server's address 1st */
	struct sockaddr_in saddr; 
	memset(&saddr, 0, sizeof(saddr)); 
	saddr.sin_family = AF_INET; 
	saddr.sin_addr.s_addr =
		((struct in_addr*) hptr->h_addr_list[0])->s_addr;
	saddr.sin_port = htons(PortNumber); /* port number in big-endian */

	if( connect( fd, (struct sockaddr*) &saddr, sizeof(saddr) ) ) {
		if( errno == EINPROGRESS ) {
			fprintf( stdout, "connect() failed: EINPROGRESS\n" );
			int result;
			socklen_t result_len = sizeof(result);
			if (getsockopt(fd, SOL_SOCKET, SO_ERROR, &result, &result_len) < 0) {
    		// error, fail somehow, close socket
				close( fd );
    		return 0;
			}
			if (result != 0) {
				// connection failed; error code is in 'result'
				fprintf( stdout, "error code: %d\n", result );
				close( fd );
				return 0;
			}
		// socket is ready for read()/write()
		}
	}	

// socket is ready for read()/write()
//

	/* Write some stuff and read the echoes. */
	puts("Connect to server, about to write some stuff..."); 

	if ( write( fd, message, strlen(message) ) > 0 ) { /* get confirmation echoed from server and print */ 
		char buffer[BuffSize + 1];
		memset(buffer, '\0', sizeof(buffer));
		if (read(fd, buffer, sizeof(buffer)) > 0) 
			puts(buffer);
	} 

	puts( "Client exit..." ); 
	close( fd ); /* close the connection */ 

	return 0;
}

// eof

