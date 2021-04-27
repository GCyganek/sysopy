//
// Created by gcyganek on 27.04.2021.
//

#include "common.h"

int shared_memory_descriptor;
pid_t *cooks;
pid_t *suppliers;
int cooks_num;
int suppliers_num;

void close_semaphore(sem_t *semaphore) {
    if (sem_close(semaphore) == -1) {
        fprintf(stderr,"Error while closing semaphore: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
}

void create_semaphore(char *semaphore_name, int semaphore_value) {
    sem_t *sem = sem_open(semaphore_name, O_CREAT | O_RDWR, 0666, semaphore_value);
    if (sem == SEM_FAILED) {
        fprintf(stderr, "Error while creating semaphore %s: %s\n", semaphore_name, strerror(errno));
        exit(EXIT_FAILURE);
    }
    close_semaphore(sem);
}

void create_semaphores() {
    create_semaphore(FURNACE_SPACE, FURNACE_CAPACITY);
    create_semaphore(TABLE_SPACE, TABLE_CAPACITY);
    create_semaphore(FURNACE_AVAILABLE, 1);
    create_semaphore(TABLE_AVAILABLE, 1);
    create_semaphore(TABLE_PIZZA_COUNTER, 0);
}

void create_shared_memory() {
    shared_memory_descriptor = shm_open(SHARED_MEMORY, O_CREAT | O_RDWR, 0666);
    if (shared_memory_descriptor == -1) {
        fprintf(stderr, "Error while creating shared memory: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (ftruncate(shared_memory_descriptor, sizeof(pizzeria)) == -1) {
        fprintf(stderr, "Error while truncating shared memory: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    pizzeria *p = mmap(NULL, sizeof(pizzeria), PROT_WRITE, MAP_SHARED, shared_memory_descriptor, 0);
    if (p == MAP_FAILED) {
        fprintf(stderr, "Error while getting shared memory info: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    p->furnace_index = -1;  // index of the pizza we want to take out from the furnace (the one that has been there the longest)
    p->table_index = -1;  // index of the pizza we want to send (the one that has been on the table the longest)
    p->pizzas_in_furnace = 0;
    p->pizzas_on_table = 0;

    munmap(p, sizeof(pizzeria));
}

void create_cooks() {
    pid_t child_pid;
    for (int i = 0; i < cooks_num; i++) {
        child_pid = fork();
        if (child_pid == -1) {
            fprintf(stderr, "Error while forking cook process: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        } else if (child_pid == 0) {
            execlp("./cook", "cook", NULL);
        }
        cooks[i] = child_pid;
    }
}

void create_suppliers() {
    pid_t child_pid;
    for (int i = 0; i < suppliers_num; i++) {
        child_pid = fork();
        if (child_pid == -1) {
            fprintf(stderr, "Error while forking supplier process: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        } else if (child_pid == 0) {
            execlp("./supplier", "supplier", NULL);
        }
        suppliers[i] = child_pid;
    }
}

void unlink_semaphore(char *semaphore_name) {
    if (sem_unlink(semaphore_name) == -1) {
        fprintf(stderr, "Error while removing %s semaphore: %s\n", semaphore_name, strerror(errno));
        exit(EXIT_FAILURE);
    }
}

void stop(int sig_no) {
    (void) sig_no;

    for (int i = 0; i < cooks_num; i++) {
        kill(cooks[i], SIGINT);
    }

    for (int i = 0; i < suppliers_num; i++) {
        kill(suppliers[i], SIGINT);
    }

    free(cooks);
    free(suppliers);

    unlink_semaphore(FURNACE_SPACE);
    unlink_semaphore(FURNACE_AVAILABLE);
    unlink_semaphore(TABLE_SPACE);
    unlink_semaphore(TABLE_AVAILABLE);
    unlink_semaphore(TABLE_PIZZA_COUNTER);

    if (shm_unlink(SHARED_MEMORY) == -1) {
        fprintf(stderr, "Error while removing shared memory: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    exit(0);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Wrong number of arguments: start the program with ./main cooks_num suppliers_num\n");
        exit(EXIT_FAILURE);
    }

    cooks_num = atoi(argv[1]);
    if (cooks_num <= 0) {
        fprintf(stderr, "Wrong argument: there should be at least 1 cook given\n");
        exit(EXIT_FAILURE);
    }

    suppliers_num = atoi(argv[2]);
    if (suppliers_num <= 0) {
        fprintf(stderr, "Wrong argument: there should be at least 1 supplier given\n");
        exit(EXIT_FAILURE);
    }

    signal(SIGINT, stop);

    cooks = malloc(sizeof(pid_t) * cooks_num);
    suppliers = malloc(sizeof(pid_t) * suppliers_num);

    create_semaphores();
    create_shared_memory();
    create_cooks();
    create_suppliers();

    for (int i = 0; i < cooks_num + suppliers_num; i++) {
        wait(NULL);
    }

    return 0;
}