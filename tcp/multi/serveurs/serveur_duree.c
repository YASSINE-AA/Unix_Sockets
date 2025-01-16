#include "utils/utils.h"
#include "services/services.h"

void *connection_handler(void *sockets)
{
    int *s = (int *)sockets;
    int service_fd = s[0];
    int sock = s[1];

    free(sockets);

    msg message = {0};
    char *args[2] = {0};

    safe_rcv(service_fd, sock, &message);

    LOG_INFO("Proxy routed message from client <%d>.", message.client_id);
    size_t args_count = get_args_from_buff(service_fd, sock, message.buff, args, sizeof(args) / sizeof(args[0]));
    get_time_taken(&message.client_connection_time, message.buff);
    safe_send(service_fd, sock, &message);

    close(service_fd);
    pthread_exit(NULL);
    return NULL;
}

int main()
{
    int sock = safe_socket();
    safe_setsockopt(sock);
    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(DUREE_PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    safe_bind(sock, &server_addr);
    safe_listen(sock, 10);

    struct sockaddr_in client_addr = {0};
    socklen_t client_addr_len = sizeof(client_addr);

    while (true)
    {
        pthread_t thread_id;
        int service_fd = safe_accept(sock, &client_addr, &client_addr_len);
        if (service_fd < 0)
        {
            perror("accept failed");
            continue;
        }

        LOG_INFO("Client with addr %s connected\n", inet_ntoa(client_addr.sin_addr));

        int *sockets = malloc(sizeof(int) * 2);
        if (!sockets)
        {
            perror("malloc failed");
            close(service_fd);
            continue;
        }
        sockets[0] = service_fd;
        sockets[1] = sock;

        if (pthread_create(&thread_id, NULL, connection_handler, (void *)sockets) < 0)
        {
            perror("could not create thread");
            free(sockets);
            close(service_fd);
            continue;
        }

        pthread_detach(thread_id);
    }

    close(sock);
    return 0;
}
