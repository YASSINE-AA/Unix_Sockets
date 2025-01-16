/*
 Copyright (c) 2025 Yassine Ahmed Ali

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include "utils.h"

size_t get_args_from_buff(int service_fd, int sock, char *buff, char *args[], size_t args_size)
{
    char *token = strtok(buff, ",");
    size_t arg_count = 0;
    while (token != NULL && arg_count < args_size)
    {
        args[arg_count++] = token;
        token = strtok(NULL, ",");
    }

    if (token != NULL && arg_count < args_size)
    {
        LOG_ERROR("Too many arguments received.");
        close(service_fd);
        close(sock);
        exit(EXIT_FAILURE);
    }

    return arg_count;
}

void safe_send(int service_fd, int sock, msg *message)
{

    if (send(service_fd, message, sizeof(*message), 0) < 0)
    {
        LOG_ERROR("Send failed");
        close(service_fd);
        if (sock != -1)
            close(sock);
        exit(EXIT_FAILURE);
    }
}

void safe_rcv(int service_fd, int sock, msg *message)
{
    if (recv(service_fd, message, sizeof(*message), 0) < 0)
    {
        LOG_ERROR("Receive failed");
        close(service_fd);
        if (sock != -1)
            close(sock);
        exit(EXIT_FAILURE);
    }
}

int safe_socket()
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        LOG_ERROR("socket() failed.");
        exit(EXIT_FAILURE);
    }
    return sock;
}

void safe_listen(int sock, int max_clients)
{
    if (listen(sock, max_clients) < 0)
    {
        close(sock);
        LOG_ERROR("Listen() failed");
        exit(EXIT_FAILURE);
    }
}

void safe_bind(int sock, struct sockaddr_in *server_addr)
{
    if (bind(sock, (struct sockaddr *)server_addr, sizeof(*server_addr)) < 0)
    {
        close(sock);
        LOG_ERROR("bind() failed");
        exit(EXIT_FAILURE);
    }
}

int safe_accept(int sock, struct sockaddr_in *client_addr, socklen_t *client_addr_len)
{
    int service_fd;
    if ((service_fd = accept(sock, (struct sockaddr *)client_addr, client_addr_len)) == -1)
    {
        LOG_ERROR("Accept failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    return service_fd;
}

void safe_setsockopt(int sock)
{
    int opt = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        close(sock);
        LOG_ERROR("setsockopt() failed");
        exit(EXIT_FAILURE);
    }
}

void concat_args(char *buff, char *args[], size_t args_size)
{
    strcpy(buff, "");
    for (size_t i = 0; i < args_size; ++i)
    {
        strcat(buff, args[i]);
        if (i != args_size - 1)
            strcat(buff, ",");
    }
}