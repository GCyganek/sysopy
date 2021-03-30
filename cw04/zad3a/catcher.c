//
// Created by gcyganek on 30.03.2021.
//

#include "header.h"

int sig1_counter = 0;
char sig2_received = 0;
enum p_mode mode;

void sig1_handler(int sig_no, siginfo_t* info, void* ucontext) {
    (void)ucontext;
    (void)sig_no;
    (void)info;

    sig1_counter++;
}

void sig2_handler(int sig_no, siginfo_t* info, void* ucontext) {
    (void)ucontext;
    (void)sig_no;

    pid_t sender_pid = info -> si_pid;

    if (mode == Sigqueue) {
        union sigval val;
        int i;

        for (i = 1; i <= sig1_counter; i++) {
            val.sival_int = i;
            sigqueue(sender_pid, SIGNAL1(mode), val);
        }

        val.sival_int = i;
        sigqueue(sender_pid, SIGNAL2(mode), val);
    }

    else {
        for (int i = 1; i <= sig1_counter; i++) {
            kill(sender_pid, SIGNAL1(mode));
        }

        kill(sender_pid, SIGNAL2(mode));
    }

    sig2_received = 1;

    printf("Catcher received %d signals from sender\n", sig1_counter);

    exit(0);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Wrong number of arguments. You should type in "
                "mode of sending signals: kill, sigqueue or sigrt\n");
        return 1;
    }

    mode = assign_mode(argv[1]);

    sigset_t set = mask_signals(sig1_handler, sig2_handler, mode);

    printf("Catcher created, pid:%d\n", getpid());

    while(!sig2_received) {
        sigsuspend(&set);
    }

    return 0;
}