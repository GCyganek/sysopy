//
// Created by gcyganek on 27.04.2021.
//

#include "common.h"

int shared_memory_descriptor;
int pizza_type;

sem_t* table_space;
sem_t* table_available;
sem_t* table_pizza_count;

void take_pizza_from_table() {
    semaphore_wait(table_pizza_count);
    semaphore_wait(table_available);
    semaphore_post(table_space);

    pizzeria *p = mmap(NULL, sizeof(pizzeria), PROT_WRITE, MAP_SHARED, shared_memory_descriptor, 0);
    if (p == NULL) {
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

    semaphore_post(table_available);
    munmap(p, sizeof(pizzeria));
}

void deliver_pizza() {
    usleep((rand() % (1000) + 4000) * 1000); // delivering
    char time[32];
    get_current_time(time);
    printf("(%d %s) Dostarczam pizze: %d\n", getpid(), time, pizza_type);
    usleep((rand() % (1000) + 4000) * 1000); // coming back
}

void open_semaphores() {
    if ((table_space = sem_open(TABLE_SPACE, O_RDWR)) == SEM_FAILED) {
        fprintf(stderr, "Error while opening semaphore /table-space: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if ((table_available = sem_open(TABLE_AVAILABLE, O_RDWR)) == SEM_FAILED) {
        fprintf(stderr, "Error while opening semaphore /furnace-space: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if ((table_pizza_count = sem_open(TABLE_PIZZA_COUNTER, O_RDWR)) == SEM_FAILED) {
        fprintf(stderr, "Error while opening semaphore /table-pizza-count: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
}

void sigint_handler(int sig_no) {
    (void) sig_no;

    close_semaphore(table_pizza_count);
    close_semaphore(table_available);
    close_semaphore(table_space);

    exit(0);
}

int main() {
    srand(getpid());

    signal(SIGINT, sigint_handler);

    open_semaphores();
    shared_memory_descriptor = open_shared_memory();

    while(1) {
        take_pizza_from_table();
        deliver_pizza();
    }
}