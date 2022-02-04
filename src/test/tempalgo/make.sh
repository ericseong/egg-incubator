#!/bin/bash

c++ -c ../../TempAlgo.cpp -std=c++11 -o TempAlgo.o
c++ -c tempalgo_test.cpp -std=c++11 -o tempalgo_test.o
c++ TempAlgo.o tempalgo_test.o -o tempalgo_test
