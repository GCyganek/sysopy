//
// Created by gcyganek on 19.04.2021.
//

#include "header.h"

char* get_home_path() {
    char* home_path = getenv("HOME");
    if (home_path == NULL) {
        fprintf(stderr, "Error while getting home path: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    return home_path;
}

key_t get_public_key()
{
    key_t key;
    if ((key = ftok(get_home_path(), 1)) == -1)
    {
        fprintf(stderr, "Error while generating public key: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    return key;
}

key_t get_private_key()
{
    key_t key;
    if ((key = ftok(get_home_path(), getpid())) == -1)
    {
        fprintf(stderr, "Error while generating private key: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    return key;
}

void send_message_to_queue(int queue_id, message* msg) {
    if (msgsnd(queue_id, msg, MAX_MESSAGE_SIZE, 0) == -1) {
        fprintf(stderr, "Error while sending message to queue: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
}

void get_message_from_queue(int queue_id, message* msg) {
    if (msgrcv(queue_id, msg, MAX_MESSAGE_SIZE, -10, 0) == -1) {
        fprintf(stderr, "Error while receiving message from queue: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
}

void get_message_from_queue_without_waiting(int queue_id, message* msg) {
    if (msgrcv(queue_id, msg, MAX_MESSAGE_SIZE, -10, IPC_NOWAIT) == -1) {
        fprintf(stderr, "Error while receiving message from queue: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
}

int create_queue(int key) {
    int queue_id;
    if ((queue_id = msgget(key, IPC_CREAT | IPC_EXCL | 0666)) == -1) {
        fprintf(stderr, "Error while creating queue: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    return queue_id;
}

void delete_queue(int queue_id) {
    if (msgctl(queue_id, IPC_RMID, NULL) == -1) {
        fprintf(stderr, "Error while deleting queue: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
}

int get_queue_id(int key) {
    int queue_id;
    if ((queue_id = msgget(key, 0)) == -1) {
        fprintf(stderr, "Error while trying to get queue id: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    return queue_id;
}

int is_queue_empty(int queue_id) {
    struct msqid_ds check;
    msgctl(queue_id, IPC_STAT, &check);
    return (check.msg_qnum > 0) ? 0 : -1;
}