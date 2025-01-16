#include "utils/utils.h"
#include "services/services.h"

int switch_connection(int op)
{

    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    switch (op)
    {
    case -1:
    {
        // AUTH
        server_addr.sin_port = htons(AUTH_PORT);
        break;
    }
    case 1:
    {
        server_addr.sin_port = htons(DATE_PORT);
        // Date
        break;
    }

    case 2:
    {
        server_addr.sin_port = htons(LS_PORT);
        // LS
        break;
    }
    case 3:
    {
        server_addr.sin_port = htons(CAT_PORT);
        // CAT
        break;
    }
    case 4:
    {
        server_addr.sin_port = htons(DUREE_PORT);
        // Date
        break;
    }

    default:
        break;
    }
    int sock = safe_socket();
    safe_connect(sock, &server_addr);
    return sock;
}

void *connection_handler(void *sockets)
{
    int *s = (int *)sockets;
    int service_fd = s[0];
    int sock = s[1];
    free(sockets);
    msg message = {0};
    char *args[2] = {0};
    while (true)
    {
        if (message.op == 5)
            break;
        safe_rcv(service_fd, sock, &message);
        int service_sock = switch_connection(message.op);
        safe_send(service_sock, sock, &message);
        safe_rcv(service_sock, sock, &message);
        safe_send(service_fd, sock, &message);
        close(service_sock);
    }

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
    server_addr.sin_port = htons(PROXY_PORT);
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
