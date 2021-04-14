//
// Created by gcyganek on 30.03.2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include <errno.h>

void sigchld_handler(int sig_no, siginfo_t* info, void* ucontext) {
    (void) sig_no;
    (void) ucontext;

    printf("Signal with no %d sent by process with pid:%d\n", info->si_signo, info->si_pid);
    printf("Child process terminated with %d value\n", info->si_status);
    printf("Child's real user id:%d\n", info->si_uid);
    printf("System time consumed:%ld\n\n", info->si_stime);
}

void sigchld_handler2(int sig_no) {
    printf("SIGCHLD %d signal handler\n", sig_no);
}

void sigfpe_handler(int sig_no, siginfo_t* info, void* ucontext) {
    (void) sig_no;
    (void) ucontext;

    printf("Signal with no %d sent by process with pid:%d\n", info->si_signo, info->si_pid);
    printf("Child's real user id:%d\n", info->si_uid);
    switch (info->si_code) {
        case FPE_INTDIV: printf("Signal code: %d - Integer divide-by-zero\n", info->si_code); break;
        case FPE_INTOVF: printf("Signal code: %d - Integer overflow\n", info->si_code); break;
        case FPE_FLTDIV: printf("Signal code: %d - Floating point divide-by-zero\n", info->si_code); break;
        case FPE_FLTOVF: printf("Signal code: %d - Floating point overflow\n", info->si_code); break;
        case FPE_FLTUND: printf("Signal code: %d - Floating point underflow\n", info->si_code); break;
        case FPE_FLTRES: printf("Signal code: %d - Floating point inexact result\n", info->si_code); break;
        case FPE_FLTINV: printf("Signal code: %d - Invalid floating point operation\n", info->si_code); break;
        case FPE_FLTSUB: printf("Signal code: %d - Subscript out of range\n", info->si_code); break;
        default: printf("Signal code: %d - ???\n", info->si_code); break;
    }
    printf("Memory location which caused fault:%p\n\n", info->si_addr);

    _exit(0);
}

void sigsegv_handler(int sig_no, siginfo_t* info, void* ucontext) {
    (void) sig_no;
    (void) ucontext;

    printf("Signal with no %d sent by process with pid:%d\n", info->si_signo, info->si_pid);
    printf("Child's real user id:%d\n", info->si_uid);
    switch (info->si_code) {
        case SEGV_MAPERR: printf("Signal code: %d - Address not mapped\n", info->si_code); break;
        case SEGV_ACCERR: printf("Signal code: %d - Invalid permissions\n", info->si_code); break;
        case SEGV_BNDERR: printf("Signal code: %d - Failed address bound check\n", info->si_code); break;
        case SEGV_PKUERR: printf("Signal code: %d - Access was denied by memory protection keys\n", info->si_code); break;
        default: printf("Signal code: %d - ???\n", info->si_code); break;
    }
    printf("Memory location which caused fault:%p\n\n", info->si_addr);

    _exit(0);
}

char nodefer_boolean = 0;

void sigusr1_handler_nodefer(int sig_no) {
    if (!nodefer_boolean) {
        nodefer_boolean = 1;
        raise(SIGUSR1);
    }
    printf("SIGUSR1 %d signal handler\n", sig_no);
}

int main(int argc, char** argv) {

    printf("\n\tSA_SIGINFO FLAG SCENARIOS\n\n");

    printf("======SCENARIO 1 WITH SIGFPE======\n");
    struct sigaction sigact_fpe;
    sigact_fpe.sa_flags = SA_SIGINFO;
    sigact_fpe.sa_sigaction = sigfpe_handler;
    sigemptyset(&sigact_fpe.sa_mask);
    if (sigaction(SIGFPE, &sigact_fpe, NULL) == -1) {
        fprintf(stderr, "Error while using sigaction(): %s", strerror(errno));
    }

    if (fork() == 0) {
       int zero = 0;
       printf("%d", 5 / zero);
    }

    wait(NULL);

    printf("======SCENARIO 2 WITH SIGSEGV======\n");
    struct sigaction sigact_segv;
    sigact_segv.sa_flags = SA_SIGINFO;
    sigact_segv.sa_sigaction = sigsegv_handler;
    sigemptyset(&sigact_segv.sa_mask);
    if (sigaction(SIGSEGV, &sigact_segv, NULL) == -1) {
        fprintf(stderr, "Error while using sigaction(): %s", strerror(errno));
    }

    if (fork() == 0) {
        char *c = "SIGSEGV";
        c[50] = 'x';
    }

    wait(NULL);

    printf("======SCENARIO 3 WITH SIGCHLD======\n");
    struct sigaction sigact_chld;
    sigact_chld.sa_flags = SA_SIGINFO;
    sigact_chld.sa_sigaction = sigchld_handler;
    sigemptyset(&sigact_chld.sa_mask);
    if (sigaction(SIGCHLD, &sigact_chld, NULL) == -1) {
        fprintf(stderr, "Error while using sigaction(): %s", strerror(errno));
    }

    if (fork() == 0) {
        return 0;
    }

    wait(NULL);

    printf("\tSA_NODEFER FLAG SCENARIO\n\n");

    signal(SIGCHLD, SIG_IGN);

    struct sigaction sigact_usr;
    sigact_usr.sa_flags = SA_NODEFER;
    sigact_usr.sa_handler = sigusr1_handler_nodefer;
    sigemptyset(&sigact_usr.sa_mask);
    if (sigaction(SIGUSR1, &sigact_usr, NULL) == -1) {
        fprintf(stderr, "Error while using sigaction(): %s", strerror(errno));
    }

    printf("======RAISING SIGUSR1 IN SIGUSR1 HANDLER ONCE======\n");
    raise(SIGUSR1);

    printf("\n\tSA_RESETHAND FLAG SCENARIO\n\n");

    struct sigaction sigact_chld2;
    sigact_chld2.sa_flags = SA_RESETHAND;
    sigact_chld2.sa_handler = sigchld_handler2;
    sigemptyset(&sigact_usr.sa_mask);
    if (sigaction(SIGCHLD, &sigact_chld2, NULL) == -1) {
        fprintf(stderr, "Error while using sigaction(): %s", strerror(errno));
    }

    printf("======FORKING TWO CHILD PROCESSES, THE SECOND ONE WILL BE IGNORED======\n");

    if (fork() == 0) {
        return 0;
    }

    wait(NULL);

    if (fork() == 0) {
        return 0;
    }

    wait(NULL);

    return 0;
}