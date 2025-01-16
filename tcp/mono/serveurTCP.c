#include "utils/utils.h"
#include "services/services.h"

void invoke_service(int op, char *buff, char *args[])
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
        get_time_taken(true, buff);
        break;
    default:
        strcpy(buff, "Invalid operation");
        break;
    }
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

    int service_fd = safe_accept(sock, &client_addr, &client_addr_len);
    get_time_taken(false, NULL); // Set start time

    LOG_INFO("Client with addr %s \n", inet_ntoa(client_addr.sin_addr));
    msg message = {0};
    message.op = -1;
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
    strcpy(message.buff, ""); // clear
    strcat(message.buff, "\n==========Operations============");
    strcat(message.buff, "\n1. Service date");
    strcat(message.buff, "\n2. Service liste fichiers");
    strcat(message.buff, "\n3. Service contenu fichier");
    strcat(message.buff, "\n4. Service duree");
    strcat(message.buff, "\n5. Quitter");
    strcat(message.buff, "\n================================");
    safe_send(service_fd, sock, &message);
    while (true)
    {
        safe_rcv(service_fd, sock, &message);
        if (message.op == 5)
            break;
        size_t args_count = get_args_from_buff(service_fd, sock, message.buff, args, sizeof(args) / sizeof(args[0]));
        invoke_service(message.op, message.buff, args);
        safe_send(service_fd, sock, &message);
    }

    close(service_fd);
    close(sock);
    return 0;
}