#!/bin/bash
# this script is to switch currently operating mode to wireless router mode.
# note that this configuration is expected to be preserved after power cycle

if [ "$EUID" -ne 0 ]
  then echo "Please run as root or via sudo.."
  exit -1 
fi

systemctl stop hostapd && systemctl disable hostapd 
systemctl stop dnsmasq && systemctl disable dnsmasq
systemctl stop wpa_supplicant && systemctl disable wpa_supplicant
systemctl stop dhcpcd && systemctl disable dhcpcd
ifconfig wlan0 down

cp -f /etc/dhcpcd.conf.ap /etc/dhcpcd.conf

ifconfig wlan0 up
systemctl enable dhcpcd && systemctl start dhcpcd
systemctl enable wpa_supplicant && systemctl start wpa_supplicant
systemctl enable dnsmasq && systemctl start dnsmasq
systemctl enable hostapd && systemctl start hostapd 
systemctl daemon-reload

exit 0

# eof
