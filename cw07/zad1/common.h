//
// Created by gcyganek on 26.04.2021.
//

#ifndef COMMON_H
#define COMMON_H

#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/time.h>
#include <unistd.h>
#include <wait.h>

#define FURNACE_CAPACITY 5
#define TABLE_CAPACITY 5

#define FURNACE_SPACE_SEMAPHORE 0
#define TABLE_SPACE_SEMAPHORE 1
#define FURNACE_AVAILABLE_SEMAPHORE 2
#define TABLE_AVAILABLE_SEMAPHORE 3
#define TABLE_PIZZAS_COUNT_SEMAPHORE 4

typedef struct pizzeria {
    int furnace_index;
    int table_index;
    int pizzas_on_table;
    int pizzas_in_furnace;
    int furnace[FURNACE_CAPACITY];
    int table[FURNACE_CAPACITY];
} pizzeria;

void get_current_time(char *time);
void operations_on_sems(int semaphores_id, struct sembuf *operations, int operations_num);
void operation_on_sem(int semaphores_id, struct sembuf *operation);

#endif
