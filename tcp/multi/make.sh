#!/bin/sh

if [ "$#" -ne 1 ]; then
    echo "Invalid arguments"
    exit 1
fi

if [ "$1" = "server" ]; then
    gcc services/services.c utils/utils.c logger.c serveurs/serveur_auth.c -o serveur_auth -I./  -Wall -Wextra -Wno-unused-variable -Wno-unused-parameter -g3 -fsanitize=address,undefined
    gcc services/services.c utils/utils.c logger.c serveurs/serveur_cat.c -o serveur_cat -I./  -Wall -Wextra -Wno-unused-variable -Wno-unused-parameter -g3 -fsanitize=address,undefined
    gcc services/services.c utils/utils.c logger.c serveurs/serveur_date.c -o serveur_date -I./  -Wall -Wextra -Wno-unused-variable -Wno-unused-parameter -g3 -fsanitize=address,undefined
    gcc services/services.c utils/utils.c logger.c serveurs/serveur_duree.c -o serveur_duree -I./  -Wall -Wextra -Wno-unused-variable -Wno-unused-parameter -g3 -fsanitize=address,undefined
    gcc services/services.c utils/utils.c logger.c serveurs/serveur_ls.c -o serveur_ls -I./  -Wall -Wextra -Wno-unused-variable -Wno-unused-parameter -g3 -fsanitize=address,undefined
    gcc services/services.c utils/utils.c logger.c proxy.c -o proxy -I./  -Wall -Wextra -Wno-unused-variable -Wno-unused-parameter -g3 -fsanitize=address,undefined

    echo "Target server built."
    exit 0
elif [ "$1" = "client" ]; then
    gcc utils/utils.c logger.c clientTCP.c -o client -I./  -Wall -Wextra -Wno-unused-variable -Wno-unused-parameter -g3 -fsanitize=address,undefined
    echo "Target client built."
    exit 0
else
    echo "Invalid target"
    exit 1
fi