//
// Created by gcyganek on 30.03.2021.
//

#include "header.h"

int sig1_counter = 0;
int sig1_to_send;
char sig2_received = 0;
enum p_mode mode;

void sig1_handler(int sig_no, siginfo_t* info, void* ucontext) {
    (void)ucontext;
    (void)sig_no;
    (void)info;

    sig1_counter++;

    if(mode == Sigqueue) {
        printf("Received sig1 no %d from catcher\n", info -> si_value.sival_int);
    }
}

void sig2_handler(int sig_no, siginfo_t* info, void* ucontext) {
    (void)ucontext;
    (void)sig_no;
    (void)info;

    printf("Sender received %d signals, sent %d\n", sig1_counter, sig1_to_send);
    sig2_received = 1;
}

void send_signals_to_catcher(pid_t catcher_pid) {
    if (mode == Sigrt) {
        union sigval val;
        int i;

        for (i = 1; i <= sig1_to_send; i++) {
            val.sival_int = i;
            sigqueue(catcher_pid, SIGNAL1(mode), val);
        }

        val.sival_int = i;
        sigqueue(catcher_pid, SIGNAL2(mode), val);
    }

    else {
        for (int i = 1; i <= sig1_to_send; i++) {
            kill(catcher_pid, SIGNAL1(mode));
        }

        kill(catcher_pid, SIGNAL2(mode));
    }
}

int main(int argc, char** argv) {
    if (argc != 4) {
        fprintf(stderr, "Wrong number of arguments. You should type in catcher pid then number of signals to "
                        "send and then mode of sending signals: kill, sigqueue or sigrt\n");
        return 1;
    }

    mode = assign_mode(argv[3]);

    sigset_t set = mask_signals(sig1_handler, sig2_handler, mode);

    printf("Sender created with pid:%d\n", getpid());

    sig1_to_send = strtol(argv[2], NULL, 10);
    pid_t catcher_pid = strtol(argv[1], NULL, 10);

    send_signals_to_catcher(catcher_pid);

    while(!sig2_received) {
        sigsuspend(&set);
    }

    return 0;
}
