//
// Created by gcyganek on 26.04.2021.
//

#include "common.h"

int semaphores_id;
int shared_memory_id;
int pizza_type;

struct sembuf decrement_pizzas_counter = {TABLE_PIZZAS_COUNT_SEMAPHORE, -1, 0};
struct sembuf get_from_table = {TABLE_SPACE_SEMAPHORE, 1, 0};
struct sembuf lock_table = {TABLE_AVAILABLE_SEMAPHORE, -1, 0};
struct sembuf unlock_table = {TABLE_AVAILABLE_SEMAPHORE, 1, 0};

void take_pizza_from_table() {
    struct sembuf operations[3] = { decrement_pizzas_counter, lock_table, get_from_table };  // wait til the table is is available and then lock it and take a pizza
    operations_on_sems(semaphores_id, operations, 3);

    pizzeria *p = shmat(shared_memory_id, NULL, 0);
    if (p == (void *) -1) {
        fprintf(stderr, "Error while getting shared memory info: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    pizza_type = p->table[p->table_index];
    p->table_index = (p->table_index + 1) % TABLE_CAPACITY;
    p->pizzas_on_table--;

    char time[32];
    get_current_time(time);
    printf("(%d %s) Pobieram pizze: %d. Liczba pizz na stole: %d.\n",
           getpid(), time, pizza_type, p->pizzas_on_table);

    operation_on_sem(semaphores_id, &unlock_table);  // we unlock the furnace
    shmdt(p);
}

void deliver_pizza() {
    usleep((rand() % (1000) + 4000) * 1000); // delivering
    char time[32];
    get_current_time(time);
    printf("(%d %s) Dostarczam pizze: %d\n", getpid(), time, pizza_type);
    usleep((rand() % (1000) + 4000) * 1000); // coming back
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

    while(1) {
        take_pizza_from_table();
        deliver_pizza();
    }
}