
// Tmp117TempSensor.cpp
// how to build: -lwiringPi is required

#include <cstring>
#include <iostream>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include "Tmp117TempSensor.h"
#include "../third_party/SparkFun_TMP117_Arduino_Library/src/SparkFun_TMP117_Registers.h"

using namespace std;

#define I2CADDR_TMP117 0x48
#define TEMP_RES 7.8125E-3 // as per tmp117 datasheet, celcius degree
#define SWAP_BYTE(x) ( ( x<<8 & 0xff00 ) | ( x>>8 & 0x00ff ) )

void Tmp117TempSensor::init() {
  int fd;

	if( initialized )
		return;

  fd = wiringPiI2CSetup(I2CADDR_TMP117);
	if( fd == -1 ) {
		cerr << "Can't open I2C device!" << endl;
		cerr << strerror(errno) << endl;
		return; // FIXME!
	}

	initialized = true;
	deviceFd = fd;
	return;
};

int Tmp117TempSensor::get( float *data ) {
	int ret = 0;

	if( !initialized ) {
		cerr << "Can't access i2c device before init()." << endl;
		return -1;
	}

	short rawData = wiringPiI2CReadReg16( deviceFd, TMP117_TEMP_RESULT );
	if( rawData == -1 ) {
		clog << "i2c read failed." << endl;
		return -1;
	}

	short d = SWAP_BYTE(rawData);
	float temp;
	if( d & 0x8000 ) { // negative degrees
		temp = -256.0 + ( d & 0x7fff ) * TEMP_RES;
	} else {
		temp = d * TEMP_RES;
	}

	*data = temp;
	return ret;
};

// EOF

