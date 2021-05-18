//
// Created by gcyganek on 17.05.2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

#define REINDEER_COUNT 9
#define DELIVERIES_TO_FINISH 30

//  mutex for ready_reindeer
pthread_mutex_t reindeer_mutex = PTHREAD_MUTEX_INITIALIZER;
//  mutex for sleeping
pthread_mutex_t sleeping_mutex = PTHREAD_MUTEX_INITIALIZER;
//  used by elves and reindeer to check if santa is sleeping so they can alert him
pthread_cond_t sleeping_cond = PTHREAD_COND_INITIALIZER;
//  used by santa to inform reindeer that they will be delivering gifts now
pthread_cond_t santa_ready_to_deliver = PTHREAD_COND_INITIALIZER;
//  used by elves and reindeer to wake up santa when there are elves waiting or reindeer ready
pthread_cond_t something_to_do = PTHREAD_COND_INITIALIZER;


int ready_reindeer = 0;
int sleeping = 0;

//  threads finish condition
char deliveries = 0;

void santa_claus() {
    while (deliveries < DELIVERIES_TO_FINISH) {
//        sleep and wait for reindeer ready
        pthread_mutex_lock(&sleeping_mutex);
        printf("Mikolaj: zasypiam\n");
        sleeping = 1;
        pthread_cond_broadcast(&sleeping_cond);
        pthread_cond_wait(&something_to_do, &sleeping_mutex);

//        wake up, there is something to do
        printf("Mikolaj: budze sie\n");
        sleeping = 0;
        pthread_mutex_unlock(&sleeping_mutex);

//        check if reindeer are ready
        pthread_mutex_lock(&reindeer_mutex);
        if (ready_reindeer == 9) {
            //            delivering gifts
            pthread_cond_broadcast(&santa_ready_to_deliver);
            pthread_mutex_unlock(&reindeer_mutex);
            printf("Mikolaj: Dostarczam zabawki\n");
            deliveries++;
            sleep(3);
        }
        else {
//            should not happen
            pthread_mutex_unlock(&reindeer_mutex);
        }
    }

    printf("Mikolaj konczy prace\n");
    pthread_exit((void *)0);
}


void reindeer(int* reindeer_id) {
    srand(*reindeer_id);

    while (deliveries < DELIVERIES_TO_FINISH) {
//        on holiday
        usleep((rand() % (5000) + 5000) * 1000);

//        holiday is over
        pthread_mutex_lock(&reindeer_mutex);
        ready_reindeer++;
        printf("Renifer (id %d): czeka %d reniferow na Mikolaja\n", *reindeer_id, ready_reindeer);
        if (ready_reindeer == 9) {
            pthread_mutex_unlock(&reindeer_mutex);
            pthread_mutex_lock(&sleeping_mutex);
            while (sleeping == 0) {
                pthread_cond_wait(&sleeping_cond, &sleeping_mutex);
            }
            printf("Renifer (id %d): Wybudzam Mikolaja\n", *reindeer_id);
            pthread_cond_broadcast(&something_to_do);
            pthread_mutex_unlock(&sleeping_mutex);
            pthread_mutex_lock(&reindeer_mutex);
            pthread_cond_wait(&santa_ready_to_deliver, &reindeer_mutex);
        }
        else {
            pthread_cond_wait(&santa_ready_to_deliver, &reindeer_mutex);
        }
        ready_reindeer--;
        pthread_mutex_unlock(&reindeer_mutex);

//       delivering gifts
        printf("Renifer (id %d): dostarczam prezenety\n", *reindeer_id);
        sleep(3);

    }

    printf("Renifer (id %d): konczy prace\n", *reindeer_id);
    pthread_exit((void *)0);
}


int main(int argc, char** argv) {

    pthread_t santa_claus_thread;
    pthread_create(&santa_claus_thread, NULL, (void* (*)(void*))santa_claus, NULL);

    pthread_t* reindeer_threads = calloc(REINDEER_COUNT, sizeof(pthread_t));
    int* reindeer_ids = calloc(REINDEER_COUNT, sizeof(int));

    for (int i = 0; i < REINDEER_COUNT; i++) {
        reindeer_ids[i] = i;
        pthread_create(&reindeer_threads[i], NULL, (void* (*)(void*))reindeer, &reindeer_ids[i]);
    }

    for (int i = 0; i < REINDEER_COUNT; i++) {
        pthread_join(reindeer_threads[i], NULL);
    }
    pthread_join(santa_claus_thread, NULL);

    free(reindeer_threads);
    free(reindeer_ids);

    pthread_mutex_destroy(&reindeer_mutex);
    pthread_mutex_destroy(&sleeping_mutex);
    pthread_cond_destroy(&sleeping_cond);
    pthread_cond_destroy(&something_to_do);
    pthread_cond_destroy(&santa_ready_to_deliver);

    return 0;
}