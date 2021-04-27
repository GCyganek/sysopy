//
// Created by gcyganek on 26.04.2021.
//

#include "common.h"

int semaphores_id;
int shared_memory_id;
int pizza_type;

struct sembuf lock_furnace = {FURNACE_AVAILABLE_SEMAPHORE, -1, 0};
struct sembuf unlock_furnace = {FURNACE_AVAILABLE_SEMAPHORE, 1, 0};
struct sembuf add_to_furnace = {FURNACE_SPACE_SEMAPHORE, -1, 0};
struct sembuf get_from_furnace = {FURNACE_SPACE_SEMAPHORE, 1, 0};

struct sembuf add_on_table = {TABLE_SPACE_SEMAPHORE, -1, 0};
struct sembuf increment_pizzas_counter = {TABLE_PIZZAS_COUNT_SEMAPHORE, 1, 0};
struct sembuf lock_table = {TABLE_AVAILABLE_SEMAPHORE, -1, 0};
struct sembuf unlock_table = {TABLE_AVAILABLE_SEMAPHORE, 1, 0};

void prepare_pizza() {
    pizza_type = rand() % 10;
    char time[32];
    get_current_time(time);
    printf("(%d %s) Przygotowuje pizze: %d\n", getpid(), time, pizza_type);
    usleep((rand() % (1000) + 1000) * 1000);
}

void pizza_to_furnace() {
    struct sembuf operations[2] = {add_to_furnace, lock_furnace};  // add_to_furnace assures us that there will be a space in furnace (if there is not, we are blocked and wait), then we will wait til another cook ends taking his pizza out and we lock the furnace
    operations_on_sems(semaphores_id, operations, 2);

    pizzeria *p = shmat(shared_memory_id, NULL, 0);
    if (p == (void *) -1) {
        fprintf(stderr, "Error while getting shared memory info: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    int current_index;
    if (p->furnace_index == -1) {
        p->furnace_index = 0;
        current_index = 0;
    } else {
        current_index = (p->furnace_index + p->pizzas_in_furnace) % FURNACE_CAPACITY;
    }

    p->furnace[current_index] = pizza_type;
    p->pizzas_in_furnace++;

    char time[32];
    get_current_time(time);
    printf("(%d %s) Dodalem pizze: %d. Liczba pizz w piecu: %d\n",
           getpid(), time, pizza_type, p->pizzas_in_furnace);

    operation_on_sem(semaphores_id, &unlock_furnace);  // we unlock the furnace
    shmdt(p);
}

void pizza_from_furnace() {
    operation_on_sem(semaphores_id, &lock_furnace);  // wait til the furnace is available and lock it, we take out a pizza from the furnace

    pizzeria *p = shmat(shared_memory_id, NULL, 0);
    if (p == (void *) -1) {
        fprintf(stderr, "Error while getting shared memory info: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    pizza_type = p->furnace[p->furnace_index];
    p->furnace_index = (p->furnace_index + 1) % FURNACE_CAPACITY;
    p->pizzas_in_furnace--;

    struct sembuf operations[2] = {get_from_furnace, unlock_furnace};  // we take the pizza and unlock the furnace
    operations_on_sems(semaphores_id, operations, 2);
    shmdt(p);
}

void pizza_on_table() {
    struct sembuf operations[3] = {add_on_table, lock_table, increment_pizzas_counter};  // wait til the table has space and then wait til its available and then lock it and also increment pizzas table counter

    operations_on_sems(semaphores_id, operations, 3);

    pizzeria *p = shmat(shared_memory_id, NULL, 0);
    if (p == (void *) -1) {
        fprintf(stderr, "Error while getting shared memory info: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    int current_index;
    if (p->table_index == -1) {
        p->table_index = 0;
        current_index = 0;
    } else {
        current_index = (p->table_index + p->pizzas_on_table) % TABLE_CAPACITY;
    }

    p->table[current_index] = pizza_type;
    p->pizzas_on_table++;

    char time[32];
    get_current_time(time);
    printf("(%d %s) Wyjmuje pizze: %d. Liczba pizz w piecu: %d. Liczba pizz na stole: %d\n",
           getpid(), time, pizza_type, p->pizzas_in_furnace, p->pizzas_on_table);

    operation_on_sem(semaphores_id, &unlock_table);  // we unlock the table
    shmdt(p);
}

int main() {
    srand(getpid());

    key_t semaphores_key = ftok("./main.c", 1);

    semaphores_id = semget(semaphores_key, 2, 0);
    if (semaphores_id == -1) {
        fprintf(stderr, "Error while trying to get semaphores: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    key_t shared_memory_key = ftok("./main.c", 2);

    shared_memory_id = shmget(shared_memory_key, sizeof(pizzeria), 0);
    if (shared_memory_id == -1) {
        fprintf(stderr, "Error while trying to get shared memory: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // its crucial to notice, that before we print info about pizzas on the table and in the furnace in function pizza_on_table
    // we take pizza from the furnace in pizza_from_furnace() and then, if we have to, we wait til the table has space to put pizza on it.
    // if it hasn't, we are blocked and in the meantime other cooks can place their pizzas into the furnace. that's why
    // when we finally put the pizza on the table we may get a different value when it comes to the pizzas in the furnace counter,
    // for example, it can be full already and that's not because of any error, it just got filled when we were waiting
    // for a space on the table
    while(1) {
        prepare_pizza();
        pizza_to_furnace();
        usleep((rand() % (1000) + 4000) * 1000);
        pizza_from_furnace();
        pizza_on_table();
    }
}