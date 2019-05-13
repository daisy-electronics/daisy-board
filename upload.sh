#!/usr/bin/env sh

arduino-cli upload --fqbn arduino:avr:mega -p /dev/ttyUSB0 $(dirname $0)/sketch/
