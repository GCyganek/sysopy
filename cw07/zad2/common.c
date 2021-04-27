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

int open_shared_memory() {
    int shared_memory_descriptor = shm_open(SHARED_MEMORY, O_RDWR, 0666);
    if (shared_memory_descriptor == -1) {
        fprintf(stderr, "Error while creating shared memory: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    return shared_memory_descriptor;
}

void semaphore_wait(sem_t *sem) {
    if (sem_wait(sem) == -1) {
        fprintf(stderr, "Error while using sem_wait in process %d: %s\n", getpid(), strerror(errno));
        exit(EXIT_FAILURE);
    }
}

void semaphore_post(sem_t *sem) {
    if (sem_post(sem) == -1) {
        fprintf(stderr, "Error while using sem_post in process %d: %s\n", getpid(), strerror(errno));
        exit(EXIT_FAILURE);
    }
}

void close_semaphore(sem_t *semaphore) {
    if (sem_close( semaphore) == -1) {
        fprintf(stderr, "Error while closing semaphore: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
}