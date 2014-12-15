#!/bin/bash

TTY="/dev/ttyUSB0"

cd "$(dirname $0)"

stty -F $TTY cs8 9600 ignbrk -brkint -icrnl -imaxbel -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echoke noflsh -ixon -crtscts
./binclock $TTY
