//
// Created by gcyganek on 15.03.2021.
//

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/times.h>

double time_in_seconds(clock_t start, clock_t end) {
    return (double) (end - start) / sysconf(_SC_CLK_TCK);
}

int get_char_function(int file_descriptor, off_t* offset) {
    char c;
    int value;
    if((value = read(file_descriptor, &c, 1)) == -1) {
        fprintf(stderr, "Error while reading file: %s\n", strerror(errno));
        exit(1);
    } else if(value == 0) {
        return EOF;
    } else {
        *offset += value;
        return (unsigned char)c;
    }
}

char* readline(off_t* offset, int file_descriptor) {
    ssize_t offset_position = lseek(file_descriptor, *offset, SEEK_SET);
    if(offset_position == -1) {
        fprintf(stderr, "Error while reading file offset position: %s\n", strerror(errno));
        return NULL;
    }

    int c;
    char* buffer = calloc(32, sizeof(char));

    size_t length = 0;
    size_t max_length = 8;

    while((c = get_char_function(file_descriptor, offset)) != EOF && c != '\n') {
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

int main(int argc, char* argv[]) {
    struct tms *start_time = malloc(sizeof(struct tms));
    struct tms *end_time = malloc(sizeof(struct tms));
    clock_t clock_start_time;
    clock_t clock_end_time;

    char file_name1[128];
    char file_name2[128];

    if(argc !=  3) {
        printf("Did not receive any file names. Please type in two file names.\n");
        printf("First file name: ");
        scanf("%s", file_name1);
        printf("Second file name: ");
        scanf("%s", file_name2);
    } else {
        if(strlen(argv[1]) > 127 || strlen(argv[2]) > 127) {
            printf("Error: file names too long\n");
            return 1;
        }
        strcpy(file_name1, argv[1]);
        strcpy(file_name2, argv[2]);
    }

    clock_start_time = times(start_time);

    int open_file1;
    int open_file2;

    open_file1 = open(file_name1, O_RDONLY);
    if(open_file1 == -1) {
        fprintf(stderr, "Error while opening file %s: %s\n", file_name1, strerror(errno));
    }
    open_file2 = open(file_name2, O_RDONLY);
    if(open_file2 == -1) {
        fprintf(stderr, "Error while opening file %s: %s\n", file_name2, strerror(errno));
    }

    char* buff1;
    char* buff2;

    off_t offset_position1 = 0;
    off_t offset_position2 = 0;

    while((buff1 = readline(&offset_position1, open_file1)) != NULL &&
          (buff2 = readline(&offset_position2, open_file2)) != NULL){
        printf("%s\n", buff1);
        printf("%s\n", buff2);
        free(buff1);
        free(buff2);
    }
    while((buff1 = readline(&offset_position1, open_file1)) != NULL) {
        printf("%s\n", buff1);
        free(buff1);
    }
    while((buff2 = readline(&offset_position2, open_file2)) != NULL) {
        printf("%s\n", buff2);
        free(buff2);
    }
    free(buff1);
    free(buff2);

    clock_end_time = times(end_time);

    FILE* report = fopen("pomiar_zad_1.txt", "a");

    fprintf(report, "\n ZAD1 FUNKCJE SYSTEMOOWE \n");
    fprintf(report, "real time:  %lf\n", time_in_seconds(clock_start_time, clock_end_time));
    fprintf(report, "user time:  %lf\n", time_in_seconds(start_time->tms_utime, end_time->tms_utime));
    fprintf(report, " sys time:  %lf\n", time_in_seconds(start_time->tms_stime, end_time->tms_stime));

    fclose(report);

    return 0;
}