#!/bin/bash

set -x

g++ -c ../../Gpio.cpp -o Gpio.o
g++ -c ../../HeatFlowActuator.cpp -o HeatFlowActuator.o
g++ -c heatflow_test.cpp -o heatflow_test.o
g++ -L/usr/local/lib Gpio.o -lwiringPi HeatFlowActuator.o heatflow_test.o -o heatflow_test

# eof

