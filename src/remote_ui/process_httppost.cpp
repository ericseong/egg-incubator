// process_httppost.cpp
// we assume we get some commands from remote ui, either as a request from browser or any type of http client
// we assume we get the command from the json formatted body of http post request
//

#ifdef DEBUG_MACOS
#include <json/json.h>
#else
#include <jsoncpp/json/json.h>
#include <systemd/sd-daemon.h>
#endif

#include <iostream>
using namespace std;

typedef enum {
	NEW_SESSION
} command_t;

static string commands[] = {
	"new_session"
};

static void process_new_session() {
#ifndef DEBUG_MACOS
	system("/home/pi/hobby/egg-incubator/src/display_server/newsession.sh");
#endif
	cerr << "newsession.sh is called!" << endl;

	return;
}

static void process_command( command_t cmd ) {
	switch( NEW_SESSION ) {
		case NEW_SESSION: 
			process_new_session();
		break;
		default: 
			cerr << "Command not found!" << endl;
		break; 
	}
	
	return;
}

// parse body and do whatever is required from the client
void handle_with_body( string& body ) {

	// read body to json root obj
	Json::Value root;
	Json::CharReaderBuilder rb;
	Json::CharReader *reader = rb.newCharReader();
	string errors;
	bool success = reader->parse( body.c_str(), body.c_str()+body.size(), &root, &errors );
	delete reader;

	if( !success ) {
		cerr << "handle_with_body: " << errors << endl;
		return;
	}

	Json::Value obj;

	// new_session
	if( root.isObject() && root.isMember( commands[0].c_str() ) ) { 
		obj = root[commands[0].c_str()];
		if( !obj.isNull() ) {
			if( obj.asString().find("true") != string::npos ) {
				process_command( NEW_SESSION );	
			}
		}
	}

	// what else?

	return;
} 

// eof

