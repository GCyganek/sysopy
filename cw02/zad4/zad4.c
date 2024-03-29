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

int get_char_function(FILE* file, off_t* offset) {
    char c;
    int value;
    if((value = fread(&c, sizeof(char), 1, file)) == 1) {
        *offset += value;
        return (unsigned char)c;
    } else if(value == 0) {
        return EOF;
    } else {
        fprintf(stderr, "Error while reading file: %s\n", strerror(errno));
        exit(1);
    }
}

char* readline(off_t* offset, FILE* file) {
    int offset_position = fseek(file, *offset, 0);
    if(offset_position != 0) {
        fprintf(stderr, "Error while reading file offset position: %s\n", strerror(errno));
        return NULL;
    }

    int c;
    char* buffer = calloc(64, sizeof(char));

    size_t length = 0;
    size_t max_length = 8;

    while((c = get_char_function(file, offset)) != EOF && c != '\n') {
        if(length + 2 >= max_length) {
            size_t new_length = max_length * 2;
            char *new_buffer = realloc(buffer, new_length);
            if(new_buffer == 0) {
                free(buffer);
                return NULL;
            }
            buffer = new_buffer;
            max_length = new_length;
        }
        buffer[length++] = c;
    }
    if(length == 0 && c == EOF) {
        return NULL;
    }

    buffer[length] = '\0';
    return buffer;
}

void write_to_file(FILE* file1, FILE* file2, char* to_change, char* change_result) {
    char* buffer;
    char* beginning;
    char* tmp_buffer;

    size_t index = 0;
    off_t offset_position = 0;
    while((buffer = readline(&offset_position, file1)) != NULL) {
        tmp_buffer = buffer;
        while((beginning = strstr(tmp_buffer, to_change)) != NULL) {
            while(tmp_buffer != beginning) {
                tmp_buffer++;
                index++;
            }
            if(index != 0) {
                char* before = calloc(index, sizeof(char));
                strncpy(before, buffer, index);

                if(fwrite(before, sizeof(char), index, file2) == -1) {
                    fprintf(stderr, "Error while writing to file: %s", strerror(errno));
                    exit(1);
                }
                free(before);
            }

            if(fwrite(change_result, sizeof(char), strlen(change_result), file2) == -1) {
                fprintf(stderr, "Error while writing to file: %s", strerror(errno));
                exit(1);
            }
            buffer += index + strlen(to_change);
            tmp_buffer += strlen(to_change);
            index = 0;
        }

        strcat(buffer, "\n");
        if(fwrite(buffer, sizeof(char), strlen(buffer), file2) == -1) {
            fprintf(stderr, "Error while writing to file: %s", strerror(errno));
            exit(1);
        }
    }
    return;
}

int main(int argc, char** argv) {
    struct tms *start_time = malloc(sizeof(struct tms));
    struct tms *end_time = malloc(sizeof(struct tms));
    clock_t clock_start_time;
    clock_t clock_end_time;

    clock_start_time = times(start_time);

    if (argc != 5) {
        fprintf(stderr, "Wrong number of arguments given\n");
        return 1;
    }

    char *file_name1 = argv[1];
    FILE* file1 = fopen(file_name1, "r");
    if (file1 == NULL) {
        fprintf(stderr, "Error while reading file %s: %s\n", file_name1, strerror(errno));
        return 1;
    }
    char *file_name2 = argv[2];
    FILE* file2 = fopen(file_name2, "w");
    if (file2 == NULL) {
        fprintf(stderr, "Error while reading file %s: %s\n", file_name2, strerror(errno));
        return 1;
    }

    char *to_change = argv[3];
    char *change_result = argv[4];
    if (strlen(to_change) > 255 || strlen(change_result) > 255) {
        fprintf(stderr, "Given strings are too long");
    }

    write_to_file(file1, file2, to_change, change_result);

    clock_end_time = times(end_time);

    FILE* report = fopen("pomiar_zad_4.txt", "a");

    fprintf(report, "\n ZAD4 FUNKCJE BIBLIOTEKI STANDARDOWEJ \n");
    fprintf(report, "real time:  %lf\n", time_in_seconds(clock_start_time, clock_end_time));
    fprintf(report, "user time:  %lf\n", time_in_seconds(start_time->tms_utime, end_time->tms_utime));
    fprintf(report, " sys time:  %lf\n", time_in_seconds(start_time->tms_stime, end_time->tms_stime));

    fclose(report);
    return 0;
}