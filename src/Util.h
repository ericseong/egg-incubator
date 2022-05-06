// Util.h
#ifndef __UTIL_H__
#define __UTIL_H__

#include <string>

struct Util {
	static std::string strFormat( const std::string& format, Args ... args );
	static std::string dateTime2Str();
};

#endif

