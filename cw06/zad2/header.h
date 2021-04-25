//
// Created by gcyganek on 20.04.2021.
//


#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <mqueue.h>

#define MAX_MESSAGE_TEXT_LEN 1024
#define QUEUE_FILENAME_MAX_LEN 10
#define SERVER_QUEUE_FILENAME "/q-server"

#define STOP 10
#define DISCONNECT 9
#define LIST 8
#define CONNECT 7
#define INIT 6
#define MESSAGE 5

typedef struct client {
    int id;
    char* queue_filename;
    mqd_t queue_descriptor;
    int connected_client_id;
    char is_available;
} client;


void send_message_to_queue(mqd_t queue_descriptor, char *msg, unsigned int type);
void get_message_from_queue(mqd_t queue_descriptor, char *msg, unsigned int *type);
void register_notification(mqd_t queue_descriptor, struct sigevent *s_sigevent);
mqd_t create_queue(char *queue_filename);
mqd_t get_queue(char *queue_filename);
void delete_queue(char *queue_filename);
void close_queue(mqd_t queue_descriptor);

#endif

