#!/bin/bash
# this needs to be run as a root or via sudo

systemctl enable dnsmasq.service
systemctl start dnsmasq.service
systemctl enable hostapd.service
systemctl start hostapd.service
cp -f /etc/dhcpcd.conf.ap /etc/dhcpcd.conf

echo "Reboot..."
reboot

# eof

