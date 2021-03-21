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
#include <math.h>
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
    char* buffer = calloc(32, sizeof(char));

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

int main(int argc, char** argv) {
    struct tms *start_time = malloc(sizeof(struct tms));
    struct tms *end_time = malloc(sizeof(struct tms));
    clock_t clock_start_time;
    clock_t clock_end_time;

    clock_start_time = times(start_time);

    FILE* data_file = fopen("dane.txt", "r");
    if(data_file == NULL) {
        fprintf(stderr, "Error while opening file data.txt: %s\n", strerror(errno));
        return 1;
    }
    FILE* file_a = fopen("asys.txt", "w");
    if(file_a == NULL) {
        fprintf(stderr, "Error while opening file a.txt: %s\n", strerror(errno));
        return 1;
    }
    FILE* file_b = fopen("bsys.txt", "w");
    if(file_b == NULL) {
        fprintf(stderr, "Error while opening file b.txt: %s\n", strerror(errno));
        return 1;
    }
    FILE* file_c = fopen("csys.txt", "w");
    if(file_c == NULL) {
        fprintf(stderr, "Error while opening file c.txt: %s\n", strerror(errno));
        return 1;
    }

    int even_numbers = 0;
    char* buffer;
    off_t offset_position = 0;
    while((buffer = readline(&offset_position, data_file)) != NULL) {
        long number = strtol(buffer, NULL, 10);
        if(number == 0 && *buffer != '0') {
            fprintf(stderr, "Error while converting string %s to long: %s\n", buffer, strerror(errno));
        } else {
            double sqr = sqrt((double) number);
            long int sqr_int = (long int) sqr;

            if(sqr_int == sqr) {
                strcat(buffer, "\n");
                fwrite(buffer, sizeof(char), strlen(buffer), file_c);
            }
        }

        if(strlen(buffer) > 1) {
            char first_digit_char = buffer[strlen(buffer) - 1];
            int first_digit_int = first_digit_char - '0';
            if(first_digit_int % 2 == 0) {
                even_numbers++;
            }
        }

        if(strlen(buffer) > 2) {
            char second_digit = buffer[strlen(buffer) - 2];
            if(second_digit == '0' || second_digit == '7') {
                strcat(buffer, "\n");
                fwrite(buffer, sizeof(char), strlen(buffer), file_b);
            }
        }
    }

    char result_for_file_a[100];
    sprintf(result_for_file_a, "Liczb parzystych jest %d", even_numbers);
    fwrite(result_for_file_a, sizeof(char), strlen(result_for_file_a), file_a);

    fclose(data_file);
    fclose(file_a);
    fclose(file_b);
    fclose(file_c);

    clock_end_time = times(end_time);

    FILE* report = fopen("pomiar_zad_3.txt", "a");

    fprintf(report, "\n ZAD3 FUNKCJE BIBLIOTEKI STANDARDOWEJ \n");
    fprintf(report, "real time:  %lf\n", time_in_seconds(clock_start_time, clock_end_time));
    fprintf(report, "user time:  %lf\n", time_in_seconds(start_time->tms_utime, end_time->tms_utime));
    fprintf(report, " sys time:  %lf\n", time_in_seconds(start_time->tms_stime, end_time->tms_stime));

    fclose(report);

    return 0;
}