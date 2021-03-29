//
// Created by gcyganek on 29.03.2021.
//

#include "header.h"
#include <wait.h>

void handler(int sig_no) {
    (void)sig_no;
    printf("SIGUSR1 received\n");
}

enum p_mode assign_mode(char* mode) {
    if (!strcmp(mode, "handler")) {
        return Handler;
    }

    else if (!strcmp(mode, "ignore")) {
        return Ignore;
    }

    else if (!strcmp(mode, "pending")) {
        return Pending;
    }

    else if (!strcmp(mode, "mask")) {
        return Mask;
    }

    fprintf(stderr, "Wrong mode, you can only choose one from: ignore, handler, pending, mask\n");
    exit(1);
}

enum p_option assign_option(char* option) {
    if (!strcmp(option, "fork")) {
        return Fork;
    }

    else if (!strcmp(option, "exec")) {
        return Exec;
    }

    fprintf(stderr, "Wrong option, you can only choose one from: fork, exec\n");
    exit(1);
}

int main(int argc, char** argv) {
    if (argc != 3) {
        fprintf(stderr, "Provide one of the modes: ignore, handler, pending, mask. "
                        "Then choose fork or exec option\n");
        return 1;
    }

    enum p_mode mode = assign_mode(argv[1]);
    enum p_option option = assign_option(argv[2]);

    // we can handle signal this way or the way shown in the next else if (it would be SIG_IGN in the place of handler)
    if (mode == Ignore) {
        signal(SIGUSR1, SIG_IGN);
    }

    else if (mode == Handler) {
        struct sigaction sigact;
        sigemptyset(&sigact.sa_mask);
        sigact.sa_handler = handler;
        sigact.sa_flags = 0;
        sigaction(SIGUSR1, &sigact, NULL);
    }

    else {
        sigset_t newmask;
        sigemptyset(&newmask);
        sigaddset(&newmask, SIGUSR1);
        if (sigprocmask(SIG_SETMASK, &newmask, NULL) == -1) {
            fprintf(stderr, "Error while blocking signal SIGUSR1\n");
            return 1;
        }
    }

    printf("Executing raise(SIGUSR1) in parent process\n");
    raise(SIGUSR1);

    if (mode == Pending || mode == Mask) {
        pending_sig_check(0);
    }

    if (option == Fork) {
        if (fork() == 0) {
            if (mode != Pending) {
                printf("Executing raise(SIGUSR1) in child process\n");
                raise(SIGUSR1);
            }

            if (mode == Pending || mode == Mask) {
                pending_sig_check(1);
            }

            return 0;
        }

        wait(NULL);
    }

    else if (option == Exec) {
        char arr_mode[2];
        sprintf(arr_mode, "%d", mode);
        execl("./exec", "./exec", arr_mode, NULL);
    }

    return 0;
}