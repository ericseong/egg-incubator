// SessionLoggerClient.h

#ifndef __SESSIONLOGGERCLIENT_H__
#define __SESSIONLOGGERCLIENT_H__ 
#include <iostream>
#include <chrono>
#include <ctime>
#include "Client.h"

class SessionLoggerClient : public Client {

public: 		
	SessionLoggerClient( std::string& host, unsigned portNo ) : Client( host, portNo ) {}
	virtual ~SessionLoggerClient() {}
	std::string getTimeStr();
	std::string getTempStr( float val );
	std::string getHumidStr( float val );
	std::string getRollerCountStr( unsigned val );
};

#endif

// EOF

