//
// Created by gcyganek on 30.03.2021.
//

#include "header.h"

sigset_t mask_signals(void (*handler1)(int, siginfo_t*, void*), void (*handler2)(int, siginfo_t*, void*), enum p_mode mode) {
    sigset_t set;
    if (sigfillset(&set) == -1) {
        fprintf(stderr, "Error while using sigfillset(): %s\n", strerror(errno));
        exit(1);
    }

    if (sigdelset(&set, SIGNAL1(mode)) == -1) {
        fprintf(stderr, "Error while using sigdelset() with SIGUSR1: %s\n", strerror(errno));
        exit(1);
    }

    if (sigdelset(&set, SIGNAL2(mode)) == -1) {
        fprintf(stderr, "Error while using sigdelset() with SIGUSR2: %s\n", strerror(errno));
        exit(1);
    }

    if (sigprocmask(SIG_BLOCK, &set, NULL) == -1) {
        fprintf(stderr, "Error while blocking signals using sigprocmask(): %s\n", strerror(errno));
        exit(1);
    }

    struct sigaction sigact_sig1;
    sigact_sig1.sa_flags = SA_SIGINFO;
    sigact_sig1.sa_sigaction = handler1;

    if (sigaction(SIGNAL1(mode), &sigact_sig1, NULL) == -1) {
        fprintf(stderr, "Error while using sigaction(): %s\n", strerror(errno));
        exit(1);
    }

    struct sigaction sigact_sig2;
    sigact_sig2.sa_flags = SA_SIGINFO;
    sigact_sig2.sa_sigaction = handler2;

    if (sigaction(SIGNAL2(mode), &sigact_sig2, NULL) == -1) {
        fprintf(stderr, "Error while using sigaction(): %s\n", strerror(errno));
        exit(1);
    }

    return set;
}

enum p_mode assign_mode(char* mode) {
    if (!strcmp(mode, "kill")) {
        return Kill;
    }

    else if (!strcmp(mode, "sigqueue")) {
        return Sigqueue;
    }

    else if (!strcmp(mode, "sigrt")) {
        return Sigrt;
    }

    fprintf(stderr, "Wrong mode, you can only choose one from: kill, sigqueue, sigrt\n");
    exit(1);
}
