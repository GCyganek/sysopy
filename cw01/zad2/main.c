//
// Created by gcyganek on 10.03.2021.
//

#include "library.h"
#include <sys/times.h>
#include <unistd.h>

double time_in_seconds(clock_t start, clock_t end) {
    return (double)(end - start) / sysconf(_SC_CLK_TCK);
}

int main(int argc, char* argv[]) {
    Table* table = NULL;

    struct tms *start_time = malloc(sizeof(struct tms));
    struct tms *end_time = malloc(sizeof(struct tms));
    clock_t clock_start_time;
    clock_t clock_end_time;

    char* command;

    int i = 1;
    while(i < argc) {
        clock_start_time = times(start_time);
        command = argv[i];

        if(strcmp(argv[i], "create_table") == 0) {
            if(table != NULL) {
                free(table);
            }

            int size = atoi(argv[++i]);
            table = create_table(size);
            i += 1;
        }

        else if(strcmp(argv[i], "merge_file_sequence") == 0) {
            merge_file_sequence(table, argv + i + 1);
            i += table->size + 1;
        }

        else if(strcmp(argv[i], "rows_in_block_count") == 0) {
            int block_id = atoi(argv[++i]);
            int result = rows_in_block_count(table, block_id);

            printf("\nrows in block nr %d: %d\n", block_id, result);
            i += 1;
        }

        else if(strcmp(argv[i], "delete_row") == 0) {
            int block_id = atoi(argv[++i]);
            int row_id = atoi(argv[++i]);
            delete_row(table, block_id, row_id);
            i += 1;
        }

        else if(strcmp(argv[i], "delete_block") == 0) {
            int block_id = atoi(argv[++i]);
            delete_block(table, block_id);
            i += 1;
        }

        else if(strcmp(argv[i], "read_table") == 0) {
            read_table(table);
            i += 1;
        }

        else if(strcmp(argv[i], "create_block") == 0) {
            create_block_from_tmp(table);
            i += 1;
        }

        else if(strcmp(argv[i], "create_and_remove_blocks") == 0) {
            int blocks_to_create = atoi(argv[++i]);
            for(int j = 0; j < blocks_to_create; j++) {
                create_block_from_tmp(table);
            }
            for(int j = 0; j < blocks_to_create; j++) {
                delete_block(table, j);
            }
            i += 1;
        }

        else if(strcmp(argv[i], "write_files_to_tmp") == 0) {
            write_files_to_tmp(argv[i + 1], argv[i + 2]);
            i += 3;
        }

        else {
            printf("Unknown command: %s\n", argv[i]);
            return 1;
        }

        clock_end_time = times(end_time);

        printf("\n  %s\n", command);
        printf("real time:  %lf\n", time_in_seconds(clock_start_time, clock_end_time));
        printf("user time:  %lf\n", time_in_seconds(start_time->tms_utime, end_time->tms_utime));
        printf(" sys time:  %lf\n", time_in_seconds(start_time->tms_stime, end_time->tms_stime));
    }

    free_table(table);
}