#include "utils/utils.h"
#include "services/services.h"

int active_clients = 0;
pthread_mutex_t client_mutex = PTHREAD_MUTEX_INITIALIZER;

void invoke_service(int op, char *buff, char *args[], int client_id)
{
    switch (op)
    {
    case 1:
        get_date_complete(buff);
        break;
    case 2:
        get_liste_fichiers(args[0], buff);
        break;
    case 3:
        get_contenu_fichier(args[0], buff);
        break;
    case 4:
        get_time_taken(true, buff, client_id);
        break;
    default:
        strcpy(buff, "Invalid operation");
        break;
    }
}

void *connection_handler(void *sockets)
{
    int *s = (int *)sockets;
    int service_fd = s[0];
    int sock = s[1];

    free(sockets); 

    pthread_mutex_lock(&client_mutex);
        int client_id = active_clients;
    get_time_taken(false, NULL, client_id); // Set start time
    pthread_mutex_unlock(&client_mutex);

    msg message = {0};
    char *args[2] = {0};

    while (true)
    {
        safe_rcv(service_fd, sock, &message);
        size_t max_args_size = sizeof(args) / sizeof(args[0]);
        size_t args_size = get_args_from_buff(service_fd, sock, message.buff, args, max_args_size);
        if (authentification(args[0], args[1]))
            break;
        safe_send(service_fd, sock, &message); // Send auth retry
    }

    message.op = 0;
    LOG_INFO("Client Authenticated Successfully!");

    // Send Menu
    snprintf(message.buff, sizeof(message.buff),
             "\n==========Operations============"
             "\n1. Service date"
             "\n2. Service liste fichiers"
             "\n3. Service contenu fichier"
             "\n4. Service duree"
             "\n5. Quitter"
             "\n================================");
    safe_send(service_fd, sock, &message);

    while (true)
    {
        safe_rcv(service_fd, sock, &message);
        if (message.op == 5)
            break;

        size_t args_count = get_args_from_buff(service_fd, sock, message.buff, args, sizeof(args) / sizeof(args[0]));
        invoke_service(message.op, message.buff, args, client_id);
        safe_send(service_fd, sock, &message);
    }

    pthread_mutex_lock(&client_mutex);
    active_clients--;
    pthread_mutex_unlock(&client_mutex);

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
    server_addr.sin_port = htons(5080);
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

        pthread_mutex_lock(&client_mutex);
        active_clients++;
        LOG_INFO("Active clients: %d", active_clients);
        pthread_mutex_unlock(&client_mutex);

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
