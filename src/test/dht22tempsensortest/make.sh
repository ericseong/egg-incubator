#!/bin/bash

set -x

g++ -c ../../Gpio.cpp -o Gpio.o
g++ -c ../../dht22.cpp -o dht22.o
g++ -c ../../Dht22TempSensor.cpp -o Dht22TempSensor.o
g++ -c dht22tempsensor_test.cpp -o dht22tempsensor_test.o
g++ -L/usr/local/lib -lwiringPi Gpio.o dht22.o Dht22TempSensor.o dht22tempsensor_test.o -o dht22tempsensor_test

# eof

