// SessionLoggerClient.h

#ifndef __SESSIONLOGGERCLIENT_H__
#define __SESSIONLOGGERCLIENT_H__ 
#include <string>
#include "Client.h"

class SessionLoggerClient : public Client {

public: 		
	SessionLoggerClient( std::string& host, unsigned portNo ) : Client( host, portNo ) {}
	virtual ~SessionLoggerClient() {}
};

#endif

// EOF

