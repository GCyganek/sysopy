//
// Created by gcyganek on 13.04.2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

char* read_file(FILE* file) {
    char c;
    char* buf = malloc(32 * sizeof(char));
    size_t length = 0;
    size_t size = 32;
    while ((c = fgetc(file)) != EOF) {
        if (length == size) {
            size *= 2;
            buf = realloc(buf, size);
            if (buf == NULL) {
                fprintf(stderr, "Error while reallocating buf in consumer: %s\n", strerror(errno));
                exit(1);
            }
        }
        buf[length++] = c;
    }

    return buf;
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Wrong number of arguments, you should provide producer file name, result file name"
                        " and row number to check\n");
        return 1;
    }

    int row_number = atoi(argv[3]);

    FILE* producer_file = fopen(argv[1], "r");
    if (producer_file == NULL) {
        fprintf(stderr, "Error while opening producer file: %s\n", strerror(errno));
        return 1;
    }

    FILE* result_file = fopen(argv[2], "r");
    if (result_file == NULL) {
        fprintf(stderr, "Error while opening result file: %s\n", strerror(errno));
        return 1;
    }

    int new_lines = row_number - 1;

    char* res_file_buf = read_file(result_file);
    char* prod_file_buf = read_file(producer_file);
    char* buf_part;

    if (row_number > 1) {
        buf_part = strtok(res_file_buf, "\n");
        if (buf_part == NULL) {
            fprintf(stderr, "Error while using strtok\n");
            return 1;
        }
        while (new_lines != 0) {
            buf_part = strtok(NULL, "\n");
            if (buf_part == NULL) {
                fprintf(stderr, "Error while using strtok\n");
                return 1;
            }
            new_lines--;
        }
    } else {
        buf_part = strtok(res_file_buf, "\n");
        if (buf_part == NULL) {
            buf_part = res_file_buf;
        }
    }
    printf("%s\n", buf_part);

    if (!strcmp(buf_part, prod_file_buf)) {
        printf("CHECKED %s - CORRECT\n", argv[1]);
    } else {
        printf("CHECKED %s - WRONG\n", argv[1]);
    }
    return 0;
}