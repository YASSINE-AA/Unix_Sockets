#include "common.h"

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        LOG_ERROR("Invalid args, pass uniquely <port>.");
        exit(EXIT_FAILURE);
    }

    int port = strtol(argv[1], NULL, 10);

    int sock;
    srand(time(NULL));
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        LOG_ERROR("Socket creation failed.");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        LOG_ERROR("Server address binding failed.");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in client_addr = {0};
    socklen_t client_addr_len;
    msg message = (msg){0};

    recvfrom(sock, &message, sizeof(message), 0, (struct sockaddr *)&client_addr, &client_addr_len);
    printf("%ld %d \n", message.value_count, message.values[0]);

    int n = message.values[0];
    message.value_count = 0;
    for (size_t i = 0; i < n; ++i)
    {
        message.values[message.value_count++] = rand() % NMAX;
    }

    sendto(sock, &message, sizeof(message), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));

    close(sock);
    return 0;
}