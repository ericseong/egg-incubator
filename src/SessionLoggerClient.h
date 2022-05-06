// SessionLoggerClient.h

#ifndef __SESSIONLOGGERCLIENT_H__
#define __SESSIONLOGGERCLIENT_H__ 
#include <string>
#include "StreamClient.h"

class SessionLoggerClient : public StreamClient {

public: 		
	SessionLoggerClient( std::string& host, unsigned portNo ) : StreamClient( host, portNo ) {}
	virtual ~SessionLoggerClient() {}
};

#endif

// EOF

