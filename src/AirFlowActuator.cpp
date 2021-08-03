// AirFlowActuator.cpp

#include <wiringPi.h>
#include <softPwm.h>
#include "gpio.h"
#include "AirFlowActuator.h"

using namespace std;

#define PWMFAN1_PIN 26 /* GPIO-12 */
#define PWMFAN2_PIN 23 /* GPIO-13 */
#define PWM_RANGE1 100 // 0~100, 0: full speed, 100: 0ff
#define PWM_RANGE2 100 // 0~100

void AirFlowActuator::set( ActStatus status ) {
}
ActStatus AirFlowActuator::get() {
}
void AirFlowActuator::start() {
} 
void AirFlowActuator::startHigh() {
}
void AirFlowActuator::startMid() {
}
void AirFlowActuator::startLow() {
}
void AirFlowActuator::stop() {
} 

// EOF

