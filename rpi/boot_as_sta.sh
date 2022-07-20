#!/bin/bash
# this needs to be run as a root or via sudo

sudo systemctl stop dnsmasq.service
sudo systemctl disable dnsmasq.service
sudo systemctl stop hostapd.service
sudo systemctl disable hostapd.service
sudo cp -f /etc/dhcpcd.conf.sta /etc/dhcpcd.conf

echo "Reboot..."
reboot

# eof

