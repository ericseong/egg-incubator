// Util.cpp

#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <chrono>
#include <ctime>
#include "Util.h"

// get the local date/time. output will be 2022-02-06 20:52
// ref: https://stackoverflow.com/questions/17223096/outputting-date-and-time-in-c-using-stdchrono
std::string Util::dateTime2Str( unsigned width ) {
  std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

  std::string s(30, '\0');
  std::strftime(&s[0], s.size(), "%Y-%m-%d %H:%M", std::localtime(&now));

	std::stringstream ss;
	ss << std::setw(width) << s;
	s = ss.str();

  return s;
}

std::string Util::fp2Str( float val, unsigned width, unsigned precision ) {
	std::stringstream ss;
	ss << std::fixed << std::setprecision(precision) << std::setw(width) << val;
	std::string s = ss.str();

	return s;
}

std::string Util::num2Str( int val, unsigned width ) {
	std::stringstream ss;
	ss << std::setw(width) << val;
	std::string s = ss.str();

	return s;
}

std::string Util::str2Str( std::string val, unsigned width ) {
	std::stringstream ss;
	ss << std::setw(width) << val;
	std::string s = ss.str();

	return s;
}

// eof

