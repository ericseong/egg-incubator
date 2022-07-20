#!/bin/bash
# this needs to be run as a root or via sudo

sudo systemctl enable dnsmasq.service
sudo systemctl start dnsmasq.service
sudo systemctl enable hostapd.service
sudo systemctl start hostapd.service
sudo cp -f /etc/dhcpcd.conf.ap /etc/dhcpcd.conf

echo "Reboot..."
reboot

# eof

