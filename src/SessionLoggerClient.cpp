// SessionLoggerClient.cpp

#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>
#include "SessionLoggerClient.h"

using namespace std;

// get the local date/time. output will be 2022-02-06 20:52 
// ref: https://stackoverflow.com/questions/17223096/outputting-date-and-time-in-c-using-stdchrono
std::string SessionLoggerClient::getTimeStr(){
	std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

	std::string s(30, '\0');
	std::strftime(&s[0], s.size(), "%Y-%m-%d %H:%M", std::localtime(&now));
	return s;
}

std::string SessionLoggerClient::getTempStr( float val ) {
	std::stringstream ss;
	ss << std::fixed << std::setprecision(2) << val;
	std::string s = ss.str();
	return s; 
}

std::string SessionLoggerClient::getHumidStr( float val ) {
	std::stringstream ss;
	ss << std::fixed << std::setprecision(2) << val;
	std::string s = ss.str();
	return s; 
}

std::string SessionLoggerClient::getRollerCountStr( unsigned val ) {
	return to_string( val ); 
}

#if 0 // for test
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


