//
// Created by gcyganek on 29.03.2021.
//

#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

enum p_mode { Ignore, Handler, Mask, Pending };
enum p_option { Fork, Exec };

void pending_sig_check(char is_child) {
    sigset_t sigset;
    sigpending(&sigset);
    if(sigismember(&sigset, SIGUSR1)) {
        printf("Pending SIGUSR1 signal in %s\n", is_child ? "child process" : "parent process");
        return;
    }
    printf("No pending signal in %s\n", is_child ? "child process" : "parent process");
    return;
}

#endif
