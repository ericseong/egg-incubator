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
#include "InfoPanel.h"

class DisplayServer {
	InfoPanel *_pIp;
	unsigned _portNo;
	unsigned _maxConnects;
	unsigned _bufSize;
	void _updateDisplay( char *msg ) const;

public:
	DisplayServer( unsigned portNo, unsigned maxConnects ) : _portNo(portNo), _maxConnects(maxConnects), _bufSize(1024) {
		_pIp = new InfoPanel();
		_pIp->init(); 
	}
	virtual ~DisplayServer() {
		if( _pIp ) {
			_pIp->deinit();
			delete _pIp;
		}
	}
	void run();
};

#endif

// EOF

