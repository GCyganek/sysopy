//
// Created by gcyganek on 27.04.2021.
//

#include "common.h"

void get_current_time(char *time) {
    struct timeval curTime;
    gettimeofday(&curTime, NULL);

    char buf[16];
    strftime(buf, 16, "%H:%M:%S", localtime(&curTime.tv_sec));

    sprintf(time, "%s:%03ld", buf, curTime.tv_usec / 1000);
}

void operations_on_sems(int semaphores_id, struct sembuf *operations, int operations_num) {
    if (semop(semaphores_id, operations, operations_num) == -1) {
        fprintf(stderr, "Error while executing operations on semaphores: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
}

void operation_on_sem(int semaphores_id, struct sembuf *operation) {
    if (semop(semaphores_id, operation, 1) == -1) {
        fprintf(stderr, "Error while executing operation on semaphore: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
}