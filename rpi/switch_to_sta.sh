#!/bin/bash
# this is to configure the rpi to run as a wireless station and to connect to Pixel_3870
# note that this configuration is preserved after power cycle

if [ "$EUID" -ne 0 ]
  then echo "Please run as root or via sudo.."
  exit -1
fi

systemctl stop hostapd && systemctl disable hostapd
systemctl stop dnsmasq && systemctl disable dnsmasq
systemctl stop wpa_supplicant && systemctl disable wpa_supplicant
systemctl stop dhcpcd && systemctl disable dhcpcd
ifconfig wlan0 down

cp -f /etc/dhcpcd.conf.sta /etc/dhcpcd.conf
systemctl daemon-reload

ifconfig wlan0 up
systemctl enable dhcpcd && systemctl start dhcpcd
systemctl enable wpa_supplicant && systemctl start wpa_supplicant
 
exit 0

# eof
