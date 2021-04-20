//
// Created by gcyganek on 19.04.2021.
//

#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>

#define MAX_MESSAGE_TEXT_LEN 1024

#define STOP 1
#define DISCONNECT 2
#define LIST 3
#define CONNECT 4
#define INIT 5
#define MESSAGE 6

typedef struct client {
    int client_id;
    int client_queue_id;
    int connected_client_id;
    char is_available;
    pid_t client_pid;
    pid_t connected_client_pid;
} client;

typedef struct message {
    long msg_type;
    char msg_text[MAX_MESSAGE_TEXT_LEN];
    int sender_id;
    pid_t sender_pid;
} message;

#define MAX_MESSAGE_SIZE sizeof(message) - sizeof(long)

void print_error_to_stderr_and_quit(char* error_msg);
char* get_home_path();
key_t get_public_key();
key_t get_private_key();
void send_message_to_queue(int queue_id, message* msg);
void get_message_from_queue(int queue_id, message* msg);
void delete_queue(int queue_id);
int create_queue(int key);
int get_queue_id(int key);
int is_queue_empty(int queue_id);
void get_message_from_queue_without_waiting(int queue_id, message* msg);

#endif
