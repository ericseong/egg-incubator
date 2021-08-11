#!/bin/bash
sudo systemctl stop 2ne1
echo "ARG1=1" > /etc/.2ne1.conf
sudo systemctl start 2ne1
sleep 10
echo "ARG1=0" > /etc/.2ne1.conf
