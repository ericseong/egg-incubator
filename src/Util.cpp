// Util.cpp

#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <chrono>
#include <ctime>
#include <memory>
#include <stdexcept>
#include "Util.h"

// from https://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
template<typename ... Args>
std::string Util::strFormat( const std::string& format, Args ... args )
{
	int size_s = std::snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
	if( size_s <= 0 ){ throw std::runtime_error( "string_format(): Error during formatting." ); }
	auto size = static_cast<size_t>( size_s );
	auto buf = std::make_unique<char[]>( size );
	std::snprintf( buf.get(), size, format.c_str(), args ... );
	return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
}

// get the local date/time. output will be 2022-02-06 20:52
// from https://stackoverflow.com/questions/17223096/outputting-date-and-time-in-c-using-stdchrono
std::string Util::dateTime2Str() {
  std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

  std::string s(30, '\0');
  std::strftime(&s[0], s.size(), "%Y-%m-%d %H:%M", std::localtime(&now));

  return s;
}

// eof

