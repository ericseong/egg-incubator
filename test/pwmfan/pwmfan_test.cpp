// pwm controlled fan test
// ref: http://wiringpi.com/reference/software-pwm-library/
// how to build: -lpthread -lwiringPi

#include <iostream>
#include <wiringPi.h>
#include <softPwm.h>

using namespace std;

#define PWMFAN1_PIN 26 /* GPIO-12 */
#define PWMFAN2_PIN 23 /* GPIO-13 */
#define PWM_RANGE1 100 // 0~100, 0: full speed, 100: 0ff
#define PWM_RANGE2 100 // 0~100 

int main() {
	if ( wiringPiSetup() == -1 ) {
		cout << "wiringPiSetup() failed. exiting.." << endl;
		exit( 1 ); 
	}
	if( softPwmCreate( PWMFAN1_PIN, 100, PWM_RANGE1 ) ) { //  pin no, init value, range
		cout << "softPwmCreate() failed. exiting.." << endl;
		exit( 1 ); 
	}
	if( softPwmCreate( PWMFAN2_PIN, 100, PWM_RANGE2 ) ) {
		cout << "softPwmCreate() failed. exiting.." << endl;
		exit( 1 ); 
	}

	int val1 = 100, val2 = 100, count=0;
	bool faster = true;

	cout << "pwm1 range " << "(0," << PWM_RANGE1 << ")" << endl;
	cout << "pwm2 range " << "(0," << PWM_RANGE2 << ")" << endl;
	while( 1 ) {
		cout << "pwm val: " << val1 << ", " << val2 << endl;
		
		softPwmWrite( PWMFAN1_PIN, val1 % PWM_RANGE1 );	
		softPwmWrite( PWMFAN2_PIN, val2 % PWM_RANGE2 );	

		delay( 200 ); // iterate for every 20 seconds
		if( faster ) {
			--val1; --val2;
		} else {
			++val1; ++val2;
		}
		if( ++count % 100 == 0 ) {
			faster == true ? faster = false : faster = true;
		}  
	}

	return 0;
}

// eof

