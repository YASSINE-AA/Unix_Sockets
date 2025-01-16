#include "common.h"

int main(int argc, char *argv[])
{
    srand(time(NULL));

    if (argc != 3)
    {
        LOG_ERROR("Invalid arguments.");
        exit(EXIT_FAILURE);
    }

    const char *nom_hote = argv[1];
    int port = strtol(argv[2], NULL, 10);
    if (port == 0)
    {
        LOG_ERROR("Invalid port value.");
        exit(EXIT_FAILURE);
    }
    // Creation serveur
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
        exit(EXIT_FAILURE);

    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(nom_hote);

    int n = rand() % NMAX;

    msg message = (msg){0};
    message.value_count = 1;
    message.values[0] = n;

    socklen_t server_addr_len;

    sendto(sock, &message, sizeof(message), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    recvfrom(sock, &message, sizeof(message), 0, (struct sockaddr *)&server_addr, &server_addr_len);

    for (size_t i = 0; i < message.value_count; ++i)
    {
        if (i == 0)
            printf("Recieved sequence of %ld numbers: \n", message.value_count);
        printf("%d \n", message.values[i]);
    }
    close(sock);
    return 0;
}