//
// Created by gcyganek on 20.04.2021.
//

#include "header.h"

void send_message_to_queue(mqd_t queue_descriptor, char *msg, unsigned int type) {
    if(mq_send(queue_descriptor, msg, strlen(msg), type) == -1) {
        fprintf(stderr, "Error while sending message to queue: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
}

void get_message_from_queue(mqd_t queue_descriptor, char *msg, unsigned int *type) {
    if(mq_receive(queue_descriptor, msg, MAX_MESSAGE_TEXT_LEN, type) == -1) {
        fprintf(stderr, "Error while receiving message from queue: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
}

void register_notification(mqd_t queue_descriptor, struct sigevent *s_sigevent) {
    if (mq_notify(queue_descriptor, s_sigevent) == -1) {
        fprintf(stderr, "Registering notification failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
}

mqd_t create_queue(char *queue_filename) {
    mqd_t queue_descriptor;

    struct mq_attr mq_attr;
    mq_attr.mq_curmsgs = 0;
    mq_attr.mq_flags = 0;
    mq_attr.mq_msgsize = MAX_MESSAGE_TEXT_LEN;
    mq_attr.mq_maxmsg = 10;

    if ((queue_descriptor = mq_open(queue_filename, O_RDONLY | O_CREAT | O_EXCL, 0666, &mq_attr)) == -1) {
        fprintf(stderr, "Error while creating queue: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    return queue_descriptor;
}

mqd_t get_queue(char *queue_filename) {
    mqd_t queue_descriptor;
    if ((queue_descriptor = mq_open(queue_filename, O_WRONLY)) == -1) {
        fprintf(stderr, "Error while creating queue: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    return queue_descriptor;
}

void delete_queue(char *queue_filename) {
    if (mq_unlink(queue_filename) == -1) {
        fprintf(stderr, "Error while deleting queue: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
}

void close_queue(mqd_t queue_descriptor) {
    if (mq_close(queue_descriptor) == -1) {
        fprintf(stderr, "Error while closing queue: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
}