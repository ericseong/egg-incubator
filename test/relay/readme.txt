gpio mode pin out
gpio write pin 0/1

gpio16 (bcm), or WiringPi 27, for motor control
gpio5 (bcm), or WiringPi 21 for bulb control

# for my configuration
gpio mode 27 out
gpio write 27 0 // motor stop
gpio write 27 1 // motor start

ref: http://wiringpi.com/the-gpio-utility/

