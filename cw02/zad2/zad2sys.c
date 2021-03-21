//
// Created by gcyganek on 17.03.2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/times.h>

double time_in_seconds(clock_t start, clock_t end) {
    return (double)(end - start) / sysconf(_SC_CLK_TCK);
}

#define MAX_ROW_LENGTH 256

size_t readline(char* buffer, long int *offset, int file) {
    ssize_t index = lseek(file, *offset, SEEK_SET);
    if(index < 0) {
        fprintf(stderr, "Error while reading file offset position: %s\n", strerror(errno));
        return -1;
    }

    index = read(file, buffer, MAX_ROW_LENGTH);
    if(index == -1) {
        fprintf(stderr, "Error while reading data from file: %s\n", strerror(errno));
    } else if (index == 0) {
        return -1;
    }
    buffer[index] = '\0';

    char *c = buffer;

    long int i = 0;
    while(i < index && *c != '\n') {
        c++;
        i++;
    }

    *offset += i;
    if(*c == '\n') {
        *offset += 1;
    } else {
        buffer[index] = '\n';
    }
    return ++i;
}

int check_char_in_row(char* row, char* special_char) {
    char *c = row;

    int i = 0;
    int row_length = strlen(row);
    while(i < row_length && *c != '\n') {
        if(*c == *special_char) {
            return 1;
        }
        i++;
        c++;
    }
    return 0;
}

int main(int argc, char** argv) {
    struct tms *start_time = malloc(sizeof(struct tms));
    struct tms *end_time = malloc(sizeof(struct tms));
    clock_t clock_start_time;
    clock_t clock_end_time;

    clock_start_time = times(start_time);

    if(argc != 3) {
        fprintf(stderr, "Wrong number of arguments given\n");
        return 1;
    }

    char* c = argv[1];
    if(strlen(c) != 1) {
        fprintf(stderr, "First argument should be a single character\n");
        return 1;
    }

    char* file_name = argv[2];
    int file = open(file_name, O_RDONLY);
    if(file == -1) {
        fprintf(stderr, "Error while opening file %s: %s\n", file_name, strerror(errno));
    }

    char row[MAX_ROW_LENGTH];
    long int offset = 0;
    while(readline(row, &offset, file) != -1) {
        strtok(row, "\n");
        if(check_char_in_row(row, c) == 1) {
            printf("%s\n", row);
        }
    }

    clock_end_time = times(end_time);

    FILE* report = fopen("pomiar_zad_2.txt", "a");

    fprintf(report, "\n ZAD2 FUNKCJE SYSTEMOWE \n");
    fprintf(report, "real time:  %lf\n", time_in_seconds(clock_start_time, clock_end_time));
    fprintf(report, "user time:  %lf\n", time_in_seconds(start_time->tms_utime, end_time->tms_utime));
    fprintf(report, " sys time:  %lf\n", time_in_seconds(start_time->tms_stime, end_time->tms_stime));

    fclose(report);

    return 0;
}
