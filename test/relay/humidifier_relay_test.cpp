// relay control test using gpio
// how to build: g++ humidifier_relay_test.cpp -lwiringPi

#include <iostream>
#include <wiringPi.h>

#define RELAY_HUMIDIFIER_PIN 22 /* GPIO-6 */

using namespace std;

int main() {

  printf( "Raspberry Pi humidifier control relay test\n" );
  if ( wiringPiSetup() == -1 )
    exit( 1 );

	pinMode( RELAY_HUMIDIFIER_PIN, OUTPUT );

	while( 1 ) {
		cout << "humidifier relay off" << endl;
		digitalWrite( RELAY_HUMIDIFIER_PIN, LOW );
		delay( 3000 ); // 1 sec
		cout << "humidifier relay on" << endl;
		digitalWrite( RELAY_HUMIDIFIER_PIN, HIGH );
		delay( 3000 );
	}

	return 0;
}

// eof
