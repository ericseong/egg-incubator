// SessionTime.cpp
//
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <string>
#include "SessionTime.h"

using namespace std;

// has session start time of ticks (1sec)
const string sBeginTimeName( "session_start.txt" );

// has the last exec. time
const string sLastTimeName( "session_last.txt" );

inline bool SessionTime::_fileExist( const string& fname ) const {
	struct stat buf;
	return ( stat( fname.c_str(), &buf ) == 0 ); 
}

inline void SessionTime::_createFile( const string& fname ) const {
	ofstream ofs(fname);
	ofs << "0"; 
	ofs.close();

	return;
}

int SessionTime::init() {

	if( !_fileExist( sBeginTimeName ) )
		_createFile( sBeginTimeName );

	if( !_fileExist( sLastTimeName ) )
		_createFile( sLastTimeName );

	string str;
	ifstream ifs( sBeginTimeName );	
	getline( ifs, str, '\0' );	
	ifs.close();
	_start = (time_t)stol( str );

	_initialized = true;
	return 0;
}

int SessionTime::deinit() {

	_initialized = false;
	return 0;
}

// get session start time tick
inline time_t SessionTime::getStart() const {
	if( _initialized )
		return _start;

	return 0;
}

// set session start time tick
void SessionTime::setStart() {
	time_t now;

	if( _initialized ) {
		time( &now );
		ofstream ofs( sBeginTimeName );
		ofs << to_string( now );
		ofs.close();
	}

	_start = now;
	return;
}


// get current time tick
inline time_t SessionTime::getCurrent() const {
	if( _initialized ) {
		time_t now;
		time( &now );
		return now;
	}

	return 0;
}

// get the last execution tick from file
inline time_t SessionTime::getLast() const {
	string str;

	if( _initialized ) {
		ifstream ifl( sLastTimeName );
  	getline( ifl, str, '\0' );
  	ifl.close();
  	return (time_t)stol( str );
	}

	return 0;
}

// set the last execution tick
inline void SessionTime::setLast() const {
	time_t now;
	time(&now);

  ofstream ofs(sLastTimeName);
  ofs << to_string(now);
  ofs.close();

	return;
}

// get elapsed tick (one sec. /tick) since session start
inline time_t SessionTime::getElapsed() const {
	if( _initialized ) {
		return ( getCurrent() - _start ); 
	}

	return 0;
}

// EOF

