// tmp117 h/w test
// how to build: g++ tmp117_test.cpp -std=c++14 -lwiringPi -o tmp117_test 

#include <iostream>
#include <iomanip>
#include <wiringPi.h>
#include <wiringPiI2C.h>

#include "../../../SparkFun_TMP117_Arduino_Library/src/SparkFun_TMP117_Registers.h"

using namespace std;

#define I2CADDR_TMP117 0x48
#define TEMP_RES 7.8125E-3 // celcius degree
#define SWAP_BYTE(x) ( ( x<<8 & 0xff00 ) | ( x>>8 & 0x00ff ) )

int main() {
	int fd;

	fd = wiringPiI2CSetup(I2CADDR_TMP117);
	cout << "Init result: " << fd << endl;

	cout << "Read result: " << endl;
	for( ; ; ) {
	//for( int i = 0 ; i < 60 ; i++ ) {
		short data = wiringPiI2CReadReg16( fd, TMP117_TEMP_RESULT );
		if( data == -1 ) {
			cout << "Read failed." << endl;
		} else {
			short data1 = SWAP_BYTE(data);	

			float cel;
			if( data1 & 0x8000 ) { // negative degrees
				cel = -256.0 + ( data1 & 0x7fff ) * TEMP_RES; 
			} else {
				cel = data1 * TEMP_RES; 
			}
			cout << std::fixed << std::setw( 5 ) << std::setprecision( 2 ) << cel << " @C " ;
			cout << "Raw data: " << "0x" << std::hex << data1 << endl;
		}
		delay( 1000 ); // 1 sec 
	}

	return 0;
}

// eof

