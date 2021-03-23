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

    pid_t child_pid;
    char** file_sequence = argv + 2;
    char* file1;
    char* file2;

    for(int i = 0; i < table_size; i++) {
        child_pid = fork();
        if(child_pid == 0) {
            file1 = strtok(file_sequence[i], ":");
            file2 = strtok(NULL, "");

            char* tmp = calloc(sizeof(char), strlen(file1) + strlen(file2) + 20);
            sprintf(tmp, "%s%s%dtmp.txt", strtok(file1, "."), strtok(file2, "."), i);

            strcat(file1, ".txt");
            strcat(file2, ".txt");

            write_files_to_tmp(file1, file2, tmp);

            create_block_from_tmp(table, tmp);
            free(tmp);
            exit(0);
        }
    }

    while (waitpid(-1, NULL, 0) != -1) {
    }

    clock_end_time = times(end_time);

    printf("real time:  %lf\n", time_in_seconds(clock_start_time, clock_end_time));
    printf("user time:  %lf\n", time_in_seconds(start_time->tms_utime, end_time->tms_utime));
    printf(" sys time:  %lf\n", time_in_seconds(start_time->tms_stime, end_time->tms_stime));

    return 0;
}