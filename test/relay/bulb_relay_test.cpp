// relay control test using gpio
// how to build: g++ bulb_relay_test.cpp -lwiringPi

#include <iostream>
#include <wiringPi.h>

#define RELAY_BULB_PIN 21 /* GPIO-5 */

using namespace std;

int main() {

  printf( "Raspberry Pi motor control relay test\n" );
  if ( wiringPiSetup() == -1 )
    exit( 1 );

	pinMode( RELAY_BULB_PIN, OUTPUT );

	while( 1 ) {
		cout << "bulb relay off" << endl;
		digitalWrite( RELAY_BULB_PIN, LOW );
		delay( 3000 ); // 1 sec
		cout << "bulb relay on" << endl;
		digitalWrite( RELAY_BULB_PIN, HIGH );
		delay( 3000 );
	}

	return 0;
}

// eof
