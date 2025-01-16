#include "utils/utils.h"
#include "services/services.h"

int sock;

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
    size_t max_args_size = sizeof(args) / sizeof(args[0]);
    size_t args_size = get_args_from_buff(service_fd, sock, message.buff, args, max_args_size);
    get_liste_fichiers(args[0], message.buff);
    safe_send(service_fd, sock, &message);
    close(service_fd);
    pthread_exit(NULL);
    return NULL;
}

void graceful_shutdown()

{
    LOG_CRITICAL("Interruption deteced. Shutting down...");
    close(sock);
    exit(EXIT_FAILURE);
}

int main()
{

    sock = safe_socket();
    signal(SIGINT, graceful_shutdown);
    safe_setsockopt(sock);
    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(LS_PORT);
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
