//
// Created by gcyganek on 13.04.2021.
//


/*
 *
 * MAX 9 PRODUCERS ALLOWED
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

int N = 5;

void run(char* file, int is_producer, int producer_id) {
    char n[10];
    sprintf(n, "%d", N);

    pid_t pid;
    pid = fork();
    if (pid == 0) {
        if (is_producer) {
            char producer[2];
            sprintf(producer, "%d", producer_id);
            if (execlp("./producer", "./producer", "fifo", producer, file, n, NULL) == -1) {
                fprintf(stderr, "Error while using execlp() in main: %s\n", strerror(errno));
            }
        } else {
            if (execlp("./consumer", "./consumer", "fifo", file, n, NULL) == -1) {
                fprintf(stderr, "Error while using execlp() in main: %s\n", strerror(errno));
            }
        }
    } else if (pid == -1)  {
        fprintf(stderr, "Error while using fork(): %s\n", strerror(errno));
        exit(1);
    }
}

int main(int argc, char *argv[]) {
    mkfifo("fifo", S_IRUSR | S_IWUSR);

    int forks = 0;

    if (argc > 1) {
        N = atoi(argv[1]);
    }

    if (argc > 2) {  // additional consumers
        int consumers = atoi(argv[2]);

        while (consumers-- > 0) {
            forks++;
            run("result.txt", 0, 0);
        }
    }

    run("result.txt", 0, 0);
    run("prod_1.txt", 1, 1);
    forks += 2;

    if ((argc == 4 && strcmp("one_producer", argv[3]) != 0) || argc < 4) {  // we can choose if we want to open 1 producer or all 5
        run("prod_2.txt", 1, 2);
        run("prod_3.txt", 1, 3);
        run("prod_4.txt", 1, 4);
        run("prod_5.txt", 1, 5);
        forks += 4;
    }

    for (int i = 0; i < forks; i++) {
        wait(NULL);
    }
    return 0;
}