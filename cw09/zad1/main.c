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

// program is running until DELIVERS_TO_FINISH deliveries are finished and PROBLEMS_TO_SOLVE are solved by santa

#define ELF_COUNT 10
#define REINDEER_COUNT 9
#define DELIVERIES_TO_FINISH 10
#define PROBLEMS_TO_SOLVE 30

//  mutex for ready_reindeer
pthread_mutex_t reindeer_mutex = PTHREAD_MUTEX_INITIALIZER;
//  mutex for waiting_elves
pthread_mutex_t elves_mutex = PTHREAD_MUTEX_INITIALIZER;
//  mutex for sleeping
pthread_mutex_t sleeping_mutex = PTHREAD_MUTEX_INITIALIZER;
//  used by elves and reindeer to check if santa is sleeping so they can alert him
pthread_cond_t sleeping_cond = PTHREAD_COND_INITIALIZER;
//  used by santa to inform reindeer that they will be delivering gifts now
pthread_cond_t santa_ready_to_deliver = PTHREAD_COND_INITIALIZER;
//  used by santa to inform elves that he will be helping them now
pthread_cond_t santa_ready_to_help = PTHREAD_COND_INITIALIZER;
//  used by elves and reindeer to wake up santa when there are elves waiting or reindeer ready
pthread_cond_t something_to_do = PTHREAD_COND_INITIALIZER;
//  used by elves to make sure that there are max 3 elves waiting in front of the santa's workshop
pthread_cond_t elves_waiting = PTHREAD_COND_INITIALIZER;

int ready_reindeer = 0;
int waiting_elves = 0;
int sleeping = 0;

//  threads finish condition
int deliveries = 0;
int problems_solved = 0;

int elves_queue[3];

void santa_claus() {
    while (deliveries < DELIVERIES_TO_FINISH || problems_solved < PROBLEMS_TO_SOLVE) {
//        sleep and wait for elves waiting or reindeer ready
        pthread_mutex_lock(&sleeping_mutex);
        printf("Mikolaj: zasypiam\n");
        sleeping = 1;
//        send the signal that you are sleeping now
        pthread_cond_broadcast(&sleeping_cond);
//        wait for the wake up signal
        pthread_cond_wait(&something_to_do, &sleeping_mutex);

//        wake up, there is something to do
        printf("Mikolaj: budze sie\n");
        sleeping = 0;
        pthread_mutex_unlock(&sleeping_mutex);

//        check if reindeer are ready
        pthread_mutex_lock(&reindeer_mutex);
        if (ready_reindeer == 9) {
//              delivering gifts...
            pthread_cond_broadcast(&santa_ready_to_deliver);
            pthread_mutex_unlock(&reindeer_mutex);
            printf("Mikolaj: Dostarczam zabawki\n");
            deliveries++;
            sleep(3);
        }

        else {
//            if reindeer are not ready, check if elves are waiting
            pthread_mutex_unlock(&reindeer_mutex);
            pthread_mutex_lock(&elves_mutex);

            if (waiting_elves == 3) {
//                help elves
                pthread_cond_broadcast(&santa_ready_to_help);
                printf("Mikolaj: Rozwiazuje problemy elfow %d %d %d\n",
                       elves_queue[0], elves_queue[1], elves_queue[2]);

                pthread_mutex_unlock(&elves_mutex);
                sleep(2);
                problems_solved++;

//                notify elves that 3 elves are coming back
                pthread_mutex_lock(&elves_mutex);
                waiting_elves = 0;
                pthread_cond_broadcast(&elves_waiting);
                pthread_mutex_unlock(&elves_mutex);
            } else {
                pthread_mutex_unlock(&elves_mutex);
                printf("Error, santa woke up when not enough elves and reindeer were waiting");
                pthread_exit((void *)1);
            }
        }
    }

    pthread_exit((void *)0);
}


//  reindeer have priority in waking santa up, so elves should not wake santa up when there are all reindeer waiting
void wait_and_wake_up_santa(int id) {
    pthread_mutex_lock(&sleeping_mutex);
    pthread_mutex_lock(&reindeer_mutex);
    while (sleeping == 0 || ready_reindeer == 9) {
        pthread_mutex_unlock(&reindeer_mutex);
        pthread_cond_wait(&sleeping_cond, &sleeping_mutex);
        pthread_mutex_lock(&reindeer_mutex);
    }
//    wake up santa, there are no reindeer waiting and he can help you solve the problem
    printf("Elf (id %d): Wybudzam Mikolaja\n", id);
    pthread_cond_broadcast(&something_to_do);
    pthread_mutex_lock(&elves_mutex);
    pthread_mutex_unlock(&sleeping_mutex);
    pthread_mutex_unlock(&reindeer_mutex);
}


void elf(int* elf_id) {
    srand(*elf_id);

    while (problems_solved < PROBLEMS_TO_SOLVE) {
        int alerted_waiting = 0;
//        work for 2-5 seconds
        usleep((rand() % (3000) + 2000) * 1000);

//        there is a problem...
        pthread_mutex_lock(&elves_mutex);
        while (waiting_elves >= 3) {
//            if there are 3 elves waiting already, wait for them to come back
            if (alerted_waiting == 0) {
//                signal that you are waiting only once
                printf("Elf (id %d): czeka na powrot elfow\n", *elf_id);
                alerted_waiting = 1;
            }
//            waiting for a signal from santa, that elves are coming back...
            pthread_cond_wait(&elves_waiting, &elves_mutex);
        }
        if (problems_solved == PROBLEMS_TO_SOLVE) {
//            santa solved the last problem, it is time to finish the job for today...
            pthread_mutex_unlock(&elves_mutex);
            pthread_exit((void *)0);
        }
        if (waiting_elves < 2) {
//            stand in the queue in front of the santa's workshop and wait for the signal from santa
            elves_queue[waiting_elves++] = *elf_id;
            printf("Elf (id %d): czeka %d elfow na Mikolaja\n", *elf_id, waiting_elves);
            pthread_cond_wait(&santa_ready_to_help, &elves_mutex);
        }
        else if (waiting_elves == 2) {
//            you are the third elf in the queue, wait for santa and wake him up if there are not all reindeer ready
            elves_queue[waiting_elves++] = *elf_id;
            printf("Elf (id %d): czeka %d elfow na Mikolaja\n", *elf_id, waiting_elves);
            pthread_mutex_unlock(&elves_mutex);
            wait_and_wake_up_santa(*elf_id);
//            you woke up santa, now wait for the signal from him to solve the problem
            pthread_cond_wait(&santa_ready_to_help, &elves_mutex);
        }
        pthread_mutex_unlock(&elves_mutex);

//        solving the problem with santa...
        printf("Elf (id %d): Mikolaj rozwiazuje problem\n", *elf_id);
        sleep(2);
    }

    pthread_exit((void *)0);
}


void reindeer(int* reindeer_id) {
    srand(*reindeer_id);

    while (deliveries < DELIVERIES_TO_FINISH) {
//        on holiday for 5-10 sec
        usleep((rand() % (5000) + 5000) * 1000);

//        holiday is over
        pthread_mutex_lock(&reindeer_mutex);
        ready_reindeer++;
        printf("Renifer (id %d): czeka %d reniferow na Mikolaja\n", *reindeer_id, ready_reindeer);
        if (ready_reindeer == 9) {
            pthread_mutex_unlock(&reindeer_mutex);
//            wait for santa to fall asleep
            pthread_mutex_lock(&sleeping_mutex);
            while (sleeping == 0) {
                pthread_cond_wait(&sleeping_cond, &sleeping_mutex);
            }
//            wake santa up
            printf("Renifer (id %d): Wybudzam Mikolaja\n", *reindeer_id);
            pthread_cond_broadcast(&something_to_do);
            pthread_mutex_unlock(&sleeping_mutex);
            pthread_mutex_lock(&reindeer_mutex);
//            wait for santa's signal to begin the delivery
            pthread_cond_wait(&santa_ready_to_deliver, &reindeer_mutex);
        }
        else {
//            if you are not the 9th reindeer, wait for santa's signal to begin the delivery
            pthread_cond_wait(&santa_ready_to_deliver, &reindeer_mutex);
        }
        ready_reindeer--;
        pthread_mutex_unlock(&reindeer_mutex);

//       delivering gifts
        sleep(3);

    }

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

    pthread_t* elf_threads = calloc(ELF_COUNT, sizeof(pthread_t));
    int* elf_ids = calloc(ELF_COUNT, sizeof(int));

    for (int i = 0; i < ELF_COUNT; i++) {
        elf_ids[i] = i;
        pthread_create(&elf_threads[i], NULL, (void* (*)(void*))elf, &elf_ids[i]);
    }

    for (int i = 0; i < REINDEER_COUNT; i++) {
        pthread_join(reindeer_threads[i], NULL);
    }
    for (int i = 0; i < ELF_COUNT; i++) {
        pthread_join(elf_threads[i], NULL);
    }
    pthread_join(santa_claus_thread, NULL);

    free(reindeer_threads);
    free(reindeer_ids);
    free(elf_threads);
    free(elf_ids);

    pthread_mutex_destroy(&reindeer_mutex);
    pthread_mutex_destroy(&elves_mutex);
    pthread_mutex_destroy(&sleeping_mutex);
    pthread_cond_destroy(&sleeping_cond);
    pthread_cond_destroy(&something_to_do);
    pthread_cond_destroy(&santa_ready_to_help);
    pthread_cond_destroy(&santa_ready_to_deliver);
    pthread_cond_destroy(&elves_waiting);

    return 0;
}