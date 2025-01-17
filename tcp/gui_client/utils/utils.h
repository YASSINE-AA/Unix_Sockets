#ifndef UTILS_H
#define UTILS_H

#include "../gooey/include/gooey.h"
#include "../common.h"

size_t get_args_from_buff(int service_fd, int sock, char *buff, char *args[], size_t args_size);

int safe_socket(void);

void safe_listen(int sock, int max_clients);

void safe_bind(int sock, struct sockaddr_in *server_addr);

int safe_accept(int sock, struct sockaddr_in *client_addr, socklen_t *client_addr_len);
void safe_setsockopt(int sock);

void safe_connect(int sock, struct sockaddr_in *server_addr);


void safe_send(int service_fd, int sock, msg *message);

void safe_rcv(int service_fd, int sock, msg *message);

void concat_args(char *buff, char *args[], size_t args_size);
#endif