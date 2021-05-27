//
// Created by gcyganek on 24.05.2021.
//

#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_MESSAGE_LENGTH 128
#define MAX_CLIENTS 16

void print_error_and_exit(char* msg);

#endif
