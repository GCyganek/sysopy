//
// Created by gcyganek on 19.04.2021.
//

#include "header.h"

void print_error_to_stderr_and_quit(char* error_msg) {
    fprintf(stderr, "%s", error_msg);
    exit(1);
}

char* get_home_path() {
    char* home_path = getenv("HOME");
    if (home_path == NULL) {
        print_error_to_stderr_and_quit(strcat("Error while getting home path: ", strerror(errno)));
    }
    return home_path;
}

key_t get_public_key()
{
    key_t key;
    if ((key = ftok(get_home_path(), 1)) == -1)
    {
        print_error_to_stderr_and_quit(strcat("Error while generating public key: ", strerror(errno)));
    }
    return key;
}

key_t get_private_key()
{
    key_t key;
    if ((key = ftok(get_home_path(), getpid())) == -1)
    {
        print_error_to_stderr_and_quit(strcat("Error while generating private key: ", strerror(errno)));
    }
    return key;
}

void send_message_to_queue(int queue_id, message* msg) {
    if (msgsnd(queue_id, msg, MAX_MESSAGE_SIZE, 0) == -1) {
        print_error_to_stderr_and_quit(strcat("Error while sending message to queue: ", strerror(errno)));
    }
}

void get_message_from_queue(int queue_id, message* msg) {
    if (msgrcv(queue_id, msg, MAX_MESSAGE_SIZE, -10, 0) == -1) {
        print_error_to_stderr_and_quit(strcat("Error while receiving message from queue: ", strerror(errno)));
    }
}

void get_message_from_queue_without_waiting(int queue_id, message* msg) {
    if (msgrcv(queue_id, msg, MAX_MESSAGE_SIZE, -10, IPC_NOWAIT) == -1) {
        print_error_to_stderr_and_quit(strcat("Error while receiving message from queue: ", strerror(errno)));
    }
}

int create_queue(int key) {
    int queue_id;
    if ((queue_id = msgget(key, IPC_CREAT | IPC_EXCL | 0666)) == -1) {
        print_error_to_stderr_and_quit(strcat("Error while creating queue: ", strerror(errno)));
    }
    return queue_id;
}

void delete_queue(int queue_id) {
    if (msgctl(queue_id, IPC_RMID, NULL) == -1) {
        print_error_to_stderr_and_quit(strcat("Error while deleting queue: ", strerror(errno)));
    }
}

int get_queue_id(int key) {
    int queue_id;
    if ((queue_id = msgget(key, 0)) == -1) {
        print_error_to_stderr_and_quit(strcat("Error while trying to get queue id: ", strerror(errno)));
    }
    return queue_id;
}

int is_queue_empty(int queue_id) {
    struct msqid_ds check;
    msgctl(queue_id, IPC_STAT, &check);
    return (check.msg_qnum > 0) ? 0 : -1;
}