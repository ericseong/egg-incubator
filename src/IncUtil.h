// IncUtil.h
#ifndef __INCUTIL_H__
#define __INCUTIL_H__

#include <string>

//struct IncUtil {
//template<typename ... Args> static std::string strFormat( const std::string& format, Args ... args );
//static std::string dateTime2Str();
//};

// strFormat() causes linker failure on rpi4 gcc v.8.x. Don't use!
template<typename ... Args> std::string strFormat( const std::string& format, Args ... args );

std::string dateTime2Str();

#endif

