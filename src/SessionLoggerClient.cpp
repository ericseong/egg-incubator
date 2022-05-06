// SessionLoggerClient.cpp

#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>
#include "SessionLoggerClient.h"

#if 0 // for test

using namespace std;
int main() {

	string host("127.0.0.1");
	SessionLoggerClient slc( host, 48558 ); 

	string str1("Hi!");
	slc.sendMsg( str1 );

	string str2("My name is Eric.");
	slc.sendMsg( str2 );

	string str3("Nice to e-meet you.");
	slc.sendMsg( str3 );

	return 0;
}

#endif

// eof


