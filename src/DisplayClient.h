// DisplayClient.h

#ifndef __DISPLAYCLIENT_H__
#define __DISPLAYCLIENT_H__ 
#include "Client.h"

class DisplayClient : public Client {

public: 		
	DisplayClient( std::string& host, unsigned portNo ) : Client( host, portNo ) {}
	virtual ~DisplayClient() {}
};

#endif

// EOF

