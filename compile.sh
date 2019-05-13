#!/usr/bin/env sh

arduino-cli compile --fqbn arduino:avr:mega:cpu=atmega2560 $(dirname $0)/sketch/
