//
// Created by gcyganek on 17.03.2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAX_ROW_LENGTH 256

int readline(char* buffer, long int *offset, FILE* file) {
    long int index = fseek(file, *offset, 0);
    if(index == -1) {
        fprintf(stderr, "Error while reading file offset position: %s\n", strerror(errno));
        return -1;
    }

    index = fread(buffer, sizeof(char), MAX_ROW_LENGTH, file);
    if(index == -1) {
        fprintf(stderr, "Error while reading data from file: %s\n", strerror(errno));
    } else if (index == 0) {
        return -1;
    }

    char *c = buffer;

    long int i = 0;
    while(i < index && *c != '\n') {
        c++;
        i++;
    }

    *offset += i + 1;
    return i;
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
    FILE* file = fopen(file_name, "r");
    if(file == NULL) {
        fprintf(stderr, "Error while opening file %s: %s\n", file_name, strerror(errno));
    }

    int result;
    char row[MAX_ROW_LENGTH];
    long int offset = 0;
    while((result = readline(row, &offset, file)) != -1) {
        strtok(row, "\n");
        if(check_char_in_row(row, c) == 1) {
            printf("%s\n", row);
        }
    }

    fclose(file);
    return 0;
}
