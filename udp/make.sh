#!/bin/sh

if [ "$#" -ne 1 ]; then
    echo "Invalid arguments"
    exit 1
fi

if [ "$1" = "server" ]; then
    gcc logger.c serveurUDP.c -o serveurUDP -I./
    echo "Target server built."
    exit 0
elif [ "$1" = "client" ]; then
    gcc logger.c clientUDP.c -o clientUDP -I./
    echo "Target client built."
    exit 0
else
    echo "Invalid target"
    exit 1
fi
