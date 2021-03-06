// DisplayServer.h

#ifndef __DISPLAYSERVER_H__
#define __DISPLAYSERVER_H__

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

int guard(int n, char * err) { if (n == -1) { perror(err); exit(1); } return n; }

class DisplayServer {
	unsigned _bufSize;
	unsigned _portNo;
	unsigned _maxConnects;
	void _updateDisplay( char *msg ) const;

public: 		
	DisplayServer( unsigned portNo, unsigned maxConnects ) : _portNo(portNo), _maxConnects(maxConnects), _bufSize(1024) {}
	virtual ~DisplayServer() {}
	void run();
};

#endif

// EOF

