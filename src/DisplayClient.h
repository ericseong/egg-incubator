// DisplayClient.h

#ifndef __DISPLAYCLIENT_H__
#define __DISPLAYCLIENT_H__ 
#include "StreamClient.h"

class DisplayClient : public StreamClient {

public: 		
	DisplayClient( std::string& host, unsigned portNo ) : StreamClient( host, portNo ) {}
	virtual ~DisplayClient() {}
};

#endif

// EOF

