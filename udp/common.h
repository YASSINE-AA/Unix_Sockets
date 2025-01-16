#ifndef COMMON_H
#define COMMON_H
#define NMAX 100


#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include "logger.h"
#include <time.h>

typedef struct
{
    int values[NMAX];
    size_t value_count;
} msg;


#endif