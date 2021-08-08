#!/bin/bash

set -x

DIR_Config=${HOME}/hobby/egg-incubator/third_party/1.3inch_LCD_HAT/c/lib/Config
DIR_EPD=${HOME}/hobby/egg-incubator/third_party/1.3inch_LCD_HAT/c/lib/LCD
DIR_FONTS=${HOME}/hobby/egg-incubator/third_party/1.3inch_LCD_HAT/c/lib/Fonts
DIR_GUI=${HOME}/hobby/egg-incubator/third_party/1.3inch_LCD_HAT/c/lib/GUI
DIR_Examples=${HOME}/hobby/egg-incubator/third_party/1.3inch_LCD_HAT/c/examples

g++ -c ../InfoPanel.cpp -I${DIR_Config} -I${DIR_EPD} -I${DIR_FONTS} -I${DIR_GUI} -I${DIR_Examples} -o InfoPanel.o
g++ lcd_test.cpp -c -I${DIR_Config} -I${DIR_EPD} -I${DIR_FONTS} -I${DIR_GUI} -I${DIR_Examples} -o lcd_test.o
g++ InfoPanel.o lcd_test.o -llcd -o lcd_test

# eof

