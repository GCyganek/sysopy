//
// Created by gcyganek on 13.04.2021.
//
#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

#define NADAWCA 0
#define DATA 1
#define SEND_MAIL 2

int main(int argc, char* argv[]) {
    int mode;
    char* email;
    char* title;
    char* body;

    if (argc == 2) {
        if (!strcmp(argv[1], "nadawca")) mode = NADAWCA;
        else if (!strcmp(argv[1], "data")) mode = DATA;
        else {
            fprintf(stderr, "Wrong arguments: sort mode should be 'nadawca' or 'data'");
            return 1;
        }
    }

    else if (argc == 4) {
        email = argv[1];
        title = argv[2];
        body = argv[3];
        mode = SEND_MAIL;
    }

    else {
        fprintf(stderr, "Wrong number of arguments\n");
        return 1;
    }

    FILE* popen_file;
    char command[1024];

    if (mode == SEND_MAIL) {
        sprintf(command, "echo '%s' | mail -s '%s' %s", body, title, email);
        if ((popen_file = popen(command, "w")) == NULL) {
            fprintf(stderr, "Error while using popen(): %s\n", strerror(errno));
            return 1;
        }
    }

    else if (mode == NADAWCA) {
        sprintf(command, "mail | sed '1d;$d' | sort -k3");
        if ((popen_file = popen(command, "w")) == NULL) {
            fprintf(stderr, "Error while using popen(): %s\n", strerror(errno));
            return 1;
        }
    }

    else if (mode == DATA) {
        sprintf(command, "mail | sed '1d;$d' | sort -k5M -k6 -k7");
        if ((popen_file = popen(command, "w")) == NULL) {
            fprintf(stderr, "Error while using popen(): %s\n", strerror(errno));
            return 1;
        }
    }

    else {
        fprintf(stderr, "Wrong mode?\n");
        return 1;
    }

    pclose(popen_file);
    return 0;
}