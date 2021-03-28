//
// Created by gcyganek on 22.03.2021.
//

#include "library.h"
#include <sys/times.h>
#include <unistd.h>
#include <sys/wait.h>

double time_in_seconds(clock_t start, clock_t end) {
    return (double)(end - start) / sysconf(_SC_CLK_TCK);
}

int main(int argc, char** argv) {
    Table* table = NULL;

    struct tms *start_time = malloc(sizeof(struct tms));
    struct tms *end_time = malloc(sizeof(struct tms));
    clock_t clock_start_time;
    clock_t clock_end_time;

    clock_start_time = times(start_time);

    int table_size = atoi(argv[1]);

    table = create_table(table_size);

    char** file_sequence = argv + 2;

    merge_file_sequence(table, file_sequence);

    read_table(table);

    clock_end_time = times(end_time);

    printf("real time:  %lf\n", time_in_seconds(clock_start_time, clock_end_time));
    printf("user time:  %lf\n", time_in_seconds(start_time->tms_utime, end_time->tms_utime));
    printf(" sys time:  %lf\n", time_in_seconds(start_time->tms_stime, end_time->tms_stime));
    printf("user time for children:  %lf\n", time_in_seconds(start_time->tms_cutime, end_time->tms_cutime));
    printf(" sys time for children:  %lf\n", time_in_seconds(start_time->tms_cstime, end_time->tms_cstime));

    free(start_time);
    free(end_time);

    return 0;
}