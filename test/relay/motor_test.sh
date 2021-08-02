#!/bin/bash

echo "gpio test.."
echo "set wiringpi #27 to output"
gpio mode 27 out
sleep 1

# low for one sec, high for 6 sec
echo "set #27 low"
gpio write 27 0 
sleep 1
echo "set #27 high"
gpio write 27 1 
sleep 6 

# low for one sec, high for 6 sec
echo "set #27 low"
gpio write 27 0 
sleep 1
echo "set #27 high"
gpio write 27 1 
sleep 6 

# low for one sec, high for 6 sec
echo "set #27 low"
gpio write 27 0 
sleep 1
echo "set #27 high"
gpio write 27 1 
sleep 6 

# stop motor
echo "set #27 low"
gpio write 27 0 

# eof

