//
// Created by gcyganek on 29.03.2021.
//

#include "header.h"

int main(int argc, char** argv) {
    if(argc != 2) {
        fprintf(stderr, "Wrong argument list given to ./exec\n");
        return 1;
    }

    enum p_mode mode = strtol(argv[1], NULL, 10);

    if (mode != Pending) {
        printf("Executing raise(SIGUSR1) in child process\n");
        raise(SIGUSR1);
    }

    if (mode == Pending || mode == Mask) {
        pending_sig_check(1);
    }

    return 0;
}