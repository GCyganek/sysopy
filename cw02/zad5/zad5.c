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

#define MAX_ROW_LENGTH 50

size_t readline(char* buffer, long int *offset, FILE* file) {
    long int index = fseek(file, *offset, 0);
    if(index == -1) {
        fprintf(stderr, "Error while reading file offset position: %s\n", strerror(errno));
        return -1;
    }

    index = fread(buffer, sizeof(char), MAX_ROW_LENGTH, file);
    if (index == 0) {
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

int main(int argc, char** argv) {
    struct tms *start_time = malloc(sizeof(struct tms));
    struct tms *end_time = malloc(sizeof(struct tms));
    clock_t clock_start_time;
    clock_t clock_end_time;

    clock_start_time = times(start_time);

    if(argc != 3) {
        fprintf(stderr, "Given number of arguments is invalid\n");
        return 1;
    }

    char* file_name_1 = argv[1];
    FILE* file_1 = fopen(file_name_1, "r");
    if(file_1 == NULL) {
        fprintf(stderr, "Error while opening file %s: %s\n", file_name_1, strerror(errno));
        return 1;
    }
    char* file_name_2 = argv[2];
    FILE* file_2 = fopen(file_name_2, "w");
    if(file_2 == NULL) {
        fprintf(stderr, "Error while opening file %s: %s\n", file_name_2, strerror(errno));
        return 1;
    }

    char row[MAX_ROW_LENGTH + 1];
    long int offset = 0;
    size_t result;
    while((result = readline(row, &offset, file_1)) != -1) {
        if(fwrite(row, sizeof(char), result, file_2) != result ) {
            fprintf(stderr, "Error while writing to the %s file: %s\n", file_name_2, strerror(errno));
        }
    }
    fclose(file_1);
    fclose(file_2);

    clock_end_time = times(end_time);

    FILE* report = fopen("pomiar_zad_5.txt", "a");

    fprintf(report, "\n ZAD5 FUNKCJE BIBLIOTEKI STANDARDOWEJ \n");
    fprintf(report, "real time:  %lf\n", time_in_seconds(clock_start_time, clock_end_time));
    fprintf(report, "user time:  %lf\n", time_in_seconds(start_time->tms_utime, end_time->tms_utime));
    fprintf(report, " sys time:  %lf\n", time_in_seconds(start_time->tms_stime, end_time->tms_stime));

    fclose(report);
    return 0;
}