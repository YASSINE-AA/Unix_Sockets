#include "utils/utils.h"
#include "services/services.h"

pthread_mutex_t client_mutex = PTHREAD_MUTEX_INITIALIZER;

int active_clients = 0;
int sock;

int route_connection(int op)
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
        LOG_INFO("Routing to Auth server");
        break;
    }
    case 1:
    {
        server_addr.sin_port = htons(DATE_PORT);
        // Date
        LOG_INFO("Routing to Date server");
        break;
    }

    case 2:
    {
        server_addr.sin_port = htons(LS_PORT);
        // LS
        LOG_INFO("Routing to ls server");
        break;
    }
    case 3:
    {
        server_addr.sin_port = htons(CAT_PORT);
        // CAT
        LOG_INFO("Routing to cat server");
        break;
    }
    case 4:
    {
        server_addr.sin_port = htons(DUREE_PORT);
        // Duree
        LOG_INFO("Routing to duree server");
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
    pthread_mutex_lock(&client_mutex);
    int client_id = active_clients;
    pthread_mutex_unlock(&client_mutex);
    struct timespec start_time = (struct timespec){0};
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    LOG_INFO("Client with id <%d> connected.", client_id);
    while (true)
    {
        safe_rcv(service_fd, sock, &message);
        if (message.op == 5)
            break;
        int service_sock = route_connection(message.op);
        message.client_id = client_id;
        message.client_connection_time = start_time;
        safe_send(service_sock, sock, &message);
        safe_rcv(service_sock, sock, &message);
        safe_send(service_fd, sock, &message);
        close(service_sock);
    }

    LOG_INFO("Client with id <%d> disconnected.", client_id);

    pthread_mutex_lock(&client_mutex);
    active_clients--;
    pthread_mutex_unlock(&client_mutex);

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

        pthread_mutex_lock(&client_mutex);
        active_clients++;
        pthread_mutex_unlock(&client_mutex);

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
