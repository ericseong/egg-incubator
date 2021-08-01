// relay control test using gpio
// how to build: g++ motor_relay_test.cpp -lwiringPi

#include <iostream>
#include <wiringPi.h>

#define RELAY_MOTOR_PIN 27 /* GPIO-16 */

using namespace std;

int main() {

  printf( "Raspberry Pi motor relay test\n" );
  if ( wiringPiSetup() == -1 )
    exit( 1 );

	pinMode( RELAY_MOTOR_PIN, OUTPUT );

	while( 1 ) {
		cout << "motor relay off" << endl;
		digitalWrite( RELAY_MOTOR_PIN, LOW );
		delay( 1000 ); // 1 sec
		cout << "motor relay on" << endl;
		digitalWrite( RELAY_MOTOR_PIN, HIGH );
		delay( 6000 );
	}

	return 0;
}

// eof
