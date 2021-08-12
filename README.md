
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND!

# Overview
egg incubator program. It'll loop endlessly to monitor/control the temperature/humidity as per the policy given by config.json.

# Managing run-time using systemd
2ne1.service will continuously monitor and control the environment. In case the service is not looping for some reasons, watchdog daemon shall restart the service for about three times. 2ne1-display-server.service shall run as a separate service and will loop to process the incoming clients' request for drawing the stats and to check if we got the key pressed. If this service fails, then it'll restart and if the restart occurs about three times in a given period, the device will be rebooted.
See "rpi/2ne1.service and 2ne1-display-server.service for the details.

# User interface
UI will simply display the days passed since the session started, temperature/humidity values gotten from the sensors, and date/time for the last update. User can start a new session by pressing button 1 and button 2 at the same time for about five seconds.

# Install and run 
The code can be compiled and run on raspberry pi. It's tested on Raspberry Pi OS Lite. To build incubator monitoring/control program, goto src/ and type make. Will get 2ne1 binary at build/ dir. To build display-server, goto src/display-server/ and type make. Will get 2ne1-display-server binay. The resulting binaries are to be registered as systemd services.
 
# Dependencies
* Enable I2C via raspi-config (for temp. sensor and RTC module)
* Enable SPI via raspi-config (for lcd display)
* Real-time clock module configuration. Used controller is ds1307
* WiringPi (See third_party/WiringPi) for gpio, i2c, pwm, and etc
* TMP117 library. See third_party/SparkFun_TMP117_Arduino_Library.tar.gz 
* Data read using dht22 temperature/humidity sensor
* LCD library. See third_party/1.3inch_LCD_HAT.7z
* libsystemd-dev for systemd watchdog
* libjsoncpp-dev
* i2c-tools

# Documents
* Circuit chematic is at doc/schematic.svg
* Class diagram is at doc/sw-design.png 

# Known issues
* Button 1 and 2 are to be used and the other buttons there on LCD HAT are not conifigured properly
* Couldn't get data from dht22 with about 50% chance of failing.

// EOF

