#!/usr/bin/env bash
# disable services
sudo systemctl stop gpsd.socket
sudo systemctl disable gpsd.socket
sudo systemctl enable gpsd.socket
sudo systemctl start gpsd.socket

#run gpsd service for serial port
stty -F /dev/ttyS0 9600
sudo gpsd /dev/ttyS0 -F /var/run/gpsd.sock
cat /dev/ttys0