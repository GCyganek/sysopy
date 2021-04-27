//
// Created by gcyganek on 27.04.2021.
//

#include "common.h"

int shared_memory_descriptor;
int pizza_type;

sem_t* furnace_space;
sem_t* furnace_available;
sem_t* table_space;
sem_t* table_available;
sem_t* table_pizza_count;

void prepare_pizza() {
    pizza_type = rand() % 10;
    char time[32];
    get_current_time(time);
    printf("(%d %s) Przygotowuje pizze: %d\n", getpid(), time, pizza_type);
    usleep((rand() % (1000) + 1000) * 1000);
}

void pizza_to_furnace() {
    semaphore_wait(furnace_space);
    semaphore_wait(furnace_available);

    pizzeria *p = mmap(NULL, sizeof(pizzeria), PROT_WRITE, MAP_SHARED, shared_memory_descriptor, 0);
    if (p == MAP_FAILED) {
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

    semaphore_post(furnace_available);
    munmap(p, sizeof(pizzeria));
}

void pizza_from_furnace() {
    semaphore_post(furnace_available);

    pizzeria *p = mmap(NULL, sizeof(pizzeria), PROT_WRITE, MAP_SHARED, shared_memory_descriptor, 0);
    if (p == MAP_FAILED) {
        fprintf(stderr, "Error while getting shared memory info: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    pizza_type = p->furnace[p->furnace_index];
    p->furnace_index = (p->furnace_index + 1) % FURNACE_CAPACITY;
    p->pizzas_in_furnace--;

    semaphore_post(furnace_space);
    semaphore_post(furnace_available);
    munmap(p, sizeof(pizzeria));
}

void pizza_on_table() {
    semaphore_wait(table_space);
    semaphore_wait(table_available);
    semaphore_post(table_pizza_count);

    pizzeria *p = mmap(NULL, sizeof(pizzeria), PROT_WRITE, MAP_SHARED, shared_memory_descriptor, 0);
    if (p == MAP_FAILED) {
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

    semaphore_post(table_available);
    munmap(p, sizeof(pizzeria));
}

void open_semaphores() {
    if ((furnace_space = sem_open(FURNACE_SPACE, O_RDWR)) == SEM_FAILED) {
        fprintf(stderr, "Error while opening semaphore /furnace-space: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if ((furnace_available = sem_open(FURNACE_AVAILABLE, O_RDWR)) == SEM_FAILED) {
        fprintf(stderr, "Error while opening semaphore /furnace-available: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

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

    close_semaphore(furnace_available);
    close_semaphore(furnace_space);
    close_semaphore(table_space);
    close_semaphore(table_available);
    close_semaphore(table_pizza_count);

    exit(0);
}

int main() {
    srand(getpid());

    signal(SIGINT, sigint_handler);

    open_semaphores();
    shared_memory_descriptor = open_shared_memory();

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