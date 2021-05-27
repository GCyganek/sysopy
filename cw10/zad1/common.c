//
// Created by gcyganek on 24.05.2021.
//

#include "common.h"

void print_error_and_exit(char* msg) {
    perror(msg);
    exit(1);
}