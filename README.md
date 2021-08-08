# This project is in WIP!!!

# Overview
egg incubator. 

incubator.service will continuously monitor and control the environment. In case the service is not looping for some reasons, incubator-watchdog.service shall recover the session by rebooting the device. 

It's with very simple user interface displaying the followings on the 1.3" display.
* days passed since session start
* measured temperature in real-time
* measured humidity in real-time

The display module is also with some buttons and each button is with its own function below:
* button 1 for initiating a new session
* button 2 for restarting the on-going session
* button 3 reserved

# Install and run 
build/egg-controller // command-line
TODO // systemd

# Known issues
* dht22 frequently fails to get the data but not always

# TODO

# Dependencies
The followings the additional configurations I did for this project via raspi-config
* Enable I2C
* Enable SPI

The following libraries has been installed during this project
* Git
* WiringPi (requires to install from source)
* i2c-tools
* libjsoncpp-dev
* library for 1.3" LCD HAT with ST77890 controller
  - https://www.waveshare.com/1.3inch-lcd-hat.htm

# Documents
* Schematic is at doc/schematic.svg
* SW design is at doc/sw_design.png 
* Doxygen format document is at doc/doxygen
 
// EOF

