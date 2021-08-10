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
	InfoPanel *_pIP;
	unsigned _portNo;
	unsigned _maxConnects;
	unsigned _bufSize;
	void _updateDisplay( char *msg ) const;

public:
	DisplayServer( unsigned portNo, unsigned maxConnects ) : _portNo(portNo), _maxConnects(maxConnects), _bufSize(1024) {
		_pIP = new InfoPanel();
		_pIP->init(); 
	}
	virtual ~DisplayServer() {
		if( _pIP ) {
			_pIP->deinit();
			delete _pIP;
		}
	}
	void run();
};

#endif

// EOF

