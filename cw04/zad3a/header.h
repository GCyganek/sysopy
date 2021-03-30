//
// Created by gcyganek on 30.03.2021.
//

#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include <errno.h>

#define SIGNAL1(mode) (mode == Sigrt ? SIGRTMIN + 1 : SIGUSR1)
#define SIGNAL2(mode) (mode == Sigrt ? SIGRTMIN + 2 : SIGUSR2)

enum p_mode { Kill, Sigqueue, Sigrt };

enum p_mode assign_mode(char* mode);
sigset_t mask_signals(void (*handler1)(int, siginfo_t*, void*), void (*handler2)(int, siginfo_t*, void*), enum p_mode mode);

#endif
