#!/bin/bash

set -x

g++ -c ../../Gpio.cpp -o Gpio.o
g++ -c ../../HumidActuator.cpp -o HumidActuator.o
g++ -c humidifier_test.cpp -o humidifier_test.o
g++ -L/usr/local/lib Gpio.o -lwiringPi HumidActuator.o humidifier_test.o -o humidifier_test

# eof

