// SessionLogger.h

#ifndef __SESSIONLOGGER_H__
#define __SESSIONLOGGER_H__ 

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
#include <string>
using namespace std;

class SessionLogger {
	unsigned _portNo;
	unsigned _maxConnects;
	const string _sBeginTimeFileName; 
	unsigned _bufSize;
	void _log( char *msg ) const;
	bool _fileExist( const string& fname ) const;
	void _createFile( const string& fname ) const;

public:
	SessionLogger( unsigned portNo, unsigned maxConnects, string sBeginTimeFileName ) : _portNo(portNo), _maxConnects(maxConnects), _sBeginTimeFileName(sBeginTimeFileName), _bufSize(1024) {}
	virtual ~SessionLogger() {}
	void run();
};

#endif

// EOF

