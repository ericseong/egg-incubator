// DisplayClient.h

#ifndef __DISPLAYCLIENT_H__
#define __DISPLAYCLIENT_H__ 

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
#include <string>

int guard(int n, char * err) { if (n == -1) { perror(err); exit(1); } return n; }

class DisplayClient {
	unsigned _bufSize;
	std::string _host;
	unsigned _portNo;

public: 		
	DisplayClient( std::string& host, unsigned portNo ) : _host(host), _portNo(portNo), _bufSize(1024) {}
	virtual ~DisplayClient() {}
	void sendMsg( std::string& msg ) const;
};

#endif

// EOF

