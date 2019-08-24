#!/bin/bash

echo "Pulling latest update"

git pull

echo "Checking required libraries are there"

if [[ ! $(ls /usr/include/libconfig.h 2> /dev/null | wc -l) = "1" ]]; then
    echo "libconfig.h is missing, please install libconfig-dev if you are using Debian based systems"
    exit 1
fi

if [[ $(ls /usr/include/X11 2> /dev/null | wc -l) = "0" ]]; then
    echo "X11 libraries are missing, please install libx11-dev if you are using Debian based systems"
    exit 1
fi

echo "Compiling the code"
gcc C/main.c C/screen.c -l X11 -l config -o ambilight

sudo cp ambilight /usr/bin/