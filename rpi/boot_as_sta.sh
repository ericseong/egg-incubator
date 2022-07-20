#!/bin/bash
# this needs to be run as a root or via sudo

systemctl stop dnsmasq.service
systemctl disable dnsmasq.service
systemctl stop hostapd.service
systemctl disable hostapd.service
cp -f /etc/dhcpcd.conf.sta /etc/dhcpcd.conf

echo "Reboot..."
reboot

# eof

