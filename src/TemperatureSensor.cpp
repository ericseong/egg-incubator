// TemperatureSensor.cpp
// how to build: -lwiringPi is required

#include <iostream>
#include <iomanip>
#include <iostream>
#include <cmath>
#include <cerrno>
#include <cstring>
#include <clocale>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include "TemperatureSensor.h"

#include "../../third_party/SparkFun_TMP117_Arduino_Library/src/SparkFun_TMP117_Registers.h"

using namespace std;

#define I2CADDR_TMP117 0x48

#define TEMP_RES 7.8125E-3 // as per tmp117 datasheet, celcius degree
#define SWAP_BYTE(x) ( ( x<<8 & 0xff00 ) | ( x>>8 & 0x00ff ) )

void TemperatureSensor::init() {
  int fd;

  fd = wiringPiI2CSetup(I2CADDR_TMP117);
	if( fd == -1 ) {
		cerr << "Can't open I2C device!" << endl;
		cerr << strerror(errno) << endl;
		return; // FIXME!
	}

	initialized = true;
	return;
};

float TemperatureSensor::get() {
	if( !initialized ) {
		cerr << "Can't access i2c device before init()." << endl;
		// FIXME!
	}

	short raw_data = wiringPiI2CReadReg16( fd, TMP117_TEMP_RESULT );
	if( data == -1 ) {
		clog << "i2c read failed." << endl;
		// FIXME!
	}

	short data = SWAP_BYTE(data);
	float temp;
	if( data & 0x8000 ) { // negative degrees
		temp = -256.0 + ( data & 0x7fff ) * TEMP_RES;
	} else {
		temp = data * TEMP_RES;
	}
		
	return temp;
};

// EOF

