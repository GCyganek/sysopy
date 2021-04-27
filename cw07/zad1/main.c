//
// Created by gcyganek on 26.04.2021.
//

#include "common.h"

int shared_memory_id;
int semaphores_id;
pid_t *cooks;
pid_t *suppliers;
int cooks_num;
int suppliers_num;

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
};

void create_semaphores() {
    key_t semaphores_key = ftok("./main.c", 1);
    if (semaphores_key == -1) {
        fprintf(stderr, "Error while generating semaphores_key: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    semaphores_id = semget(semaphores_key, 5, IPC_CREAT | IPC_EXCL | 0666);
    if (semaphores_id == -1) {
        fprintf(stderr, "Error while creating semaphores: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    union semun arg;
    arg.val = 5;

    if (semctl(semaphores_id, FURNACE_SPACE_SEMAPHORE, SETVAL, arg) == -1) {
        fprintf(stderr, "Error while setting semaphore value: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (semctl(semaphores_id, TABLE_SPACE_SEMAPHORE, SETVAL, arg) == -1) {
        fprintf(stderr, "Error while setting semaphore value: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    arg.val = 1;

    if (semctl(semaphores_id, FURNACE_AVAILABLE_SEMAPHORE, SETVAL, arg) == -1) {
        fprintf(stderr, "Error while setting semaphore value: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (semctl(semaphores_id, TABLE_AVAILABLE_SEMAPHORE, SETVAL, arg) == -1) {
        fprintf(stderr, "Error while setting semaphore value: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    arg.val = 0;

    if (semctl(semaphores_id, TABLE_PIZZAS_COUNT_SEMAPHORE, SETVAL, arg) == -1) {
        fprintf(stderr, "Error while setting semaphore value: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
}

void create_shared_memory() {
    key_t shared_memory_key = ftok("./main.c", 2);
    if (shared_memory_key == -1) {
        fprintf(stderr, "Error while generating shared_memory_key: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    shared_memory_id = shmget(shared_memory_key, sizeof(pizzeria), IPC_CREAT | IPC_EXCL | 0666);
    if (shared_memory_id == -1) {
        fprintf(stderr, "Error while creating shared memory: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    pizzeria *p = shmat(shared_memory_id, NULL, 0);
    if (p == NULL) {
        fprintf(stderr, "Error while getting shared memory info: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    p->furnace_index = -1;  // index of the pizza we want to take out from the furnace (the one that has been there the longest)
    p->table_index = -1;  // index of the pizza we want to send (the one that has been on the table the longest)
    p->pizzas_in_furnace = 0;
    p->pizzas_on_table = 0;

    shmdt(p);
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

    if (semctl(semaphores_id, 0, IPC_RMID, NULL) == -1) {
        fprintf(stderr, "Error while removing semaphores: %s\n", strerror(errno));
    }
    if (shmctl(shared_memory_id, IPC_RMID, NULL) == -1) {
        fprintf(stderr, "Error while removing shared memory: %s\n", strerror(errno));
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

    stop(SIGINT);
}