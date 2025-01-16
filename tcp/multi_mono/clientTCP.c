#include "utils/utils.h"

int main(int argc, char *argv[])
{
    int sock = safe_socket();

    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    safe_connect(sock, &server_addr);

    msg message = {0};
    message.op = -1;
    while (message.op < 0)
    {
        // Authenticate
        char user[64] = {0}, pass[64] = {0};

        printf("\n==========Authentication============\n");
        printf("\nType username: ");
        scanf("%63s", user);
        printf("\nType password: ");
        scanf("%63s", pass);
        printf("\n====================================\n");

        // Prepare arguments
        char *args[2] = {user, pass};
        concat_args(message.buff, args, sizeof(args) / sizeof(args[0]));

        // Send authentication details
        safe_send(sock, -1, &message);
        safe_rcv(sock, -1, &message);
        if (message.op < 0)
        {
            LOG_ERROR("Username and/or password incorrect.");
        }
    }

    LOG_INFO("You have been successfully authenticated!");
    printf("%s", message.buff);

    bool running = true;
    while (running)
    {
        // Choose service
        printf("\nRun: ");
        scanf("%d", &message.op);

        if (message.op == 5)
        {
            safe_send(sock, -1, &message); // send client termination
            break;
        }

        // Get arguments
        printf("Args (seperated by ','): ");
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
            ;
        fgets(message.buff, sizeof(message.buff), stdin);
        message.buff[strcspn(message.buff, "\n")] = 0;
        safe_send(sock, -1, &message);
        safe_rcv(sock, -1, &message);
        printf("\noperation result: \n %s", message.buff);
    }

    close(sock);
    return 0;
}
