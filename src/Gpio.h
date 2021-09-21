// Gpio.h 

#ifndef __GPIO_H__
#define __GPIO_H__ 
#include "Singleton.h"

// gpio pin numbering by wiringPi configuration 
// ref: https://www.digikey.com/en/maker/blogs/2019/how-to-use-gpio-on-the-raspberry-pi-with-c
//#define PWMFAN1_PIN 					26 /* GPIO-12 */
//#define PWMFAN2_PIN 					23 /* GPIO-13 */
#define PWMFAN1_PIN 					23 /* GPIO-12 */
#define PWMFAN2_PIN 					26 /* GPIO-13 */
#define RELAY_BULB1_PIN 				21 /* GPIO-5 */
//#define RELAY_BULB2_PIN 	22 /* GPIO-6 */
#define RELAY_HEATFLOW_PIN 	22 /* GPIO-6 */
#define RELAY_MOTOR_PIN 			27 /* GPIO-16 */
#define RELAY_HUMIDIFIER_PIN 4 /* GPIO-23 */
#define RELAY_HUMIDIFIER_POWER_PIN 5 /* GPIO-24 */

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

