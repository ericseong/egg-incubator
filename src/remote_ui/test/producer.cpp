// producer.cpp
// produces stat file with random values. this is to test remote_ui.

// stat file format shall be..
/*
  format string for 10 items, or 11 space separated fields (space is there between date and time)
    elapsed_tick,
    date/time,
    temp,
    temp_high,
    temp_low,
    humid,
    humid_high,
    humid_low,
    roller_count,
    bool(periodic_hen_leaving)
**/

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>
#include "../../LockedFileAccess.h"
using namespace std;

// generate random floating number between low and high
static float rand_float( float low, float high ) {
	return low + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(high-low)));
}

int main() {

	string line;
	ostringstream oss;
	time_t since, now;
  struct tm *timeptr;
	float temp, temp_high, temp_low;
	float humid, humid_high, humid_low;
	unsigned roller_count = 0;
	bool phl = false;
	char c[100];

	time(&since);

	char fname[256]="../";
	strcat(fname, CUR_SESSION_STAT_FILE_NAME);
	LockedFileAccess lfa( fname );

	while(1) {

		line.clear();
		time(&now);

		// elapsed tick
		oss << now-since << " "; 
		line += oss.str();
		oss.str("");
		
		// date/time
		timeptr = localtime(&now);
		strftime( c, sizeof(c), "%Y-%m-%d %H:%M", timeptr ); // 2022-03-05 17:09
		line += c; line += " ";

		// temp, temp_high, temp_low
		oss.precision(2);
		oss << std::fixed << rand_float(36.5,38.7) << " " << "37.7"  << " " << "37.5" << " "; 
		line += oss.str();
		oss.str("");

		// humid, humid_high, humid_low
		oss.precision(2);
		oss << std::fixed << rand_float(38.0, 67.0) << " " << "57.0" << " " << "48.0" << " "; 
		line += oss.str();
		oss.str("");

		// roller count
		oss << roller_count++%1000 << " ";
		line += oss.str();
		oss.str("");

		// periodic hen leaving
		oss << (phl ? "true" : "false") << " "; 
		phl = ( phl ? false : true ); // toggle
		line += oss.str();
		oss.str("");

		// write it into file
		lfa.writeFile( line ); 

		std::this_thread::sleep_for(std::chrono::seconds(3));
	}

	return 0;
}

// eof

