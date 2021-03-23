//
// Created by gcyganek on 22.03.2021.
//

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char** argv) {
    if(argc != 2) {
        fprintf(stderr, "Wrong number of arguments\n");
        return 1;
    }

    pid_t child_pid;
    long child_process_num = strtol(argv[1], NULL, 10);
    if(child_process_num < 1) {
        fprintf(stderr, "Wrong argument value, number of child processes should be more than 0, %ld given\n", child_process_num);
        return 1;
    }

    while(child_process_num--) {
        child_pid = fork();
        if(child_pid == 0) {
            printf("Message from child process pid:%d with parent process pid:%d\n", (int)getpid(), (int)getppid());
            exit(0);
        }
    }

    while (waitpid(-1, NULL, 0) != -1) {
    }

    return 0;
}