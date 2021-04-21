//
// Created by gcyganek on 20.04.2021.
//

#include "header.h"

void print_error_to_stderr_and_quit(char *error_msg) {
    fprintf(stderr, "%s", error_msg);
    exit(1);
}

void send_message_to_queue(mqd_t queue_descriptor, char *msg, unsigned int type) {
    if(mq_send(queue_descriptor, msg, strlen(msg), type) == -1) {
        print_error_to_stderr_and_quit(strcat("Error while sending message to queue: %s\n",
                                              strerror(errno)));
    }
}

void get_message_from_queue(mqd_t queue_descriptor, char *msg, unsigned int *type) {
    if(mq_receive(queue_descriptor, msg, MAX_MESSAGE_TEXT_LEN, type) == -1) {
        print_error_to_stderr_and_quit(strcat("Error while receiving message from queue: %s\n",
                                              strerror(errno)));
    }
}

void register_notification(mqd_t queue_descriptor, struct sigevent *s_sigevent) {
    if (mq_notify(queue_descriptor, s_sigevent) == -1) {
        print_error_to_stderr_and_quit(strcat("Registering notification failed: %s\n", strerror(errno)));
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
        print_error_to_stderr_and_quit(strcat("Error while creating queue: %s\n", strerror(errno)));
    }
    return queue_descriptor;
}

mqd_t get_queue(char *queue_filename) {
    mqd_t queue_descriptor;
    if ((queue_descriptor = mq_open(queue_filename, O_WRONLY)) == -1) {
        print_error_to_stderr_and_quit(strcat("Error while creating queue: %s\n", strerror(errno)));
    }
    return queue_descriptor;
}

void delete_queue(char *queue_filename) {
    if (mq_unlink(queue_filename) == -1) {
        print_error_to_stderr_and_quit(strcat("Error while deleting queue: %s\n", strerror(errno)));
    }
}

void close_queue(mqd_t queue_descriptor) {
    if (mq_close(queue_descriptor) == -1) {
        print_error_to_stderr_and_quit(strcat("Error while closing queue: %s\n", strerror(errno)));
    }
}