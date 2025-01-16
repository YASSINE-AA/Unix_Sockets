#!/bin/sh

if [ "$#" -ne 1 ]; then
    echo "Invalid arguments"
    exit 1
fi

if [ "$1" = "server" ]; then
    gcc services/services.c utils/utils.c logger.c serveurTCP.c -o serveur -I./
    echo "Target server built."
    exit 0
elif [ "$1" = "client" ]; then
    gcc utils/utils.c logger.c clientTCP.c -o client -I./
    echo "Target client built."
    exit 0
else
    echo "Invalid target"
    exit 1
fi