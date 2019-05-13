#!/usr/bin/env sh

arduino-cli upload --fqbn arduino:avr:mega:cpu=atmega2560 -p /dev/ttyUSB0 $(dirname $0)/sketch/
