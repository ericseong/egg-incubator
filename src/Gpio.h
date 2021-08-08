// Gpio.h 

#ifndef __GPIO_H__
#define __GPIO_H__ 
#include "Singleton.h"

// gpio pin numbering by wiringPi configuration 
#define PWMFAN1_PIN 					26 /* GPIO-12 */
#define PWMFAN2_PIN 					23 /* GPIO-13 */
#define RELAY_BULB_PIN 				21 /* GPIO-5 */
#define RELAY_HUMIDIFIER_PIN 	22 /* GPIO-6 */
#define RELAY_MOTOR_PIN 			27 /* GPIO-16 */

/** TODO! Make sure that gpio pins used is not duplicately used in any of the following extension boards
 * USB/Ethernet b/d 
 * lcd HAT 
 * RTC module
 */

class Gpio : public Singleton<Gpio> {
	bool _initialized;

public:
	Gpio() : _initialized(false) {}
	~Gpio() { _initialized = false; }
	void init();
	void deinit();
};

#endif

