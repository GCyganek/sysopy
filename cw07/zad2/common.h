//
// Created by gcyganek on 27.04.2021.
//

#ifndef COMMON_H
#define COMMON_H

#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include <unistd.h>
#include <wait.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#define FURNACE_CAPACITY 5
#define TABLE_CAPACITY 5

#define SHARED_MEMORY "/shared-memory"

#define FURNACE_SPACE "/furnace-space"
#define FURNACE_AVAILABLE "/furnace-available"
#define TABLE_SPACE "/table-space"
#define TABLE_AVAILABLE "/table-available"
#define TABLE_PIZZA_COUNTER "/table-pizza-counter"

typedef struct pizzeria {
    int furnace_index;
    int table_index;
    int pizzas_on_table;
    int pizzas_in_furnace;
    int furnace[FURNACE_CAPACITY];
    int table[FURNACE_CAPACITY];
} pizzeria;

void get_current_time(char *time);
int open_shared_memory();
void semaphore_wait(sem_t *sem);
void semaphore_post(sem_t *sem);
void close_semaphore(sem_t *semaphore);

#endif
