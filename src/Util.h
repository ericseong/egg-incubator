// Util.h
#ifndef __UTIL_H__
#define __UTIL_H__

#include <string>

class Util {
public:
	static std::string dateTime2Str( unsigned width );
	static std::string fp2Str( float val, unsigned width, unsigned precision );
	static std::string num2Str( int val, unsigned width );
	static std::string str2Str( std::string val, unsigned width );
};

#endif

