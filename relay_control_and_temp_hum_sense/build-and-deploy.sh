#!/bin/sh -e

arduino-cli compile -b arduino:avr:nano . --library DHT11/ -u -p /dev/serial/by-id/usb-1a86_USB_Serial-if00-port0
