//
// Created by gcyganek on 10.05.2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <errno.h>
#include <sys/times.h>
#include <time.h>


#define MAX_LINE_LENGTH 70

int threads_num;
int height;
int width;

unsigned char **image;
unsigned char **negative;

int min(int a, int b) {
    return a > b ? b : a;
}

double get_time(struct timespec *start) {
    struct timespec end;
    clock_gettime(CLOCK_MONOTONIC, &end);
    return (end.tv_sec - start->tv_sec) * 1e6 + (end.tv_nsec - start->tv_nsec) / 1e3;
}

void read_line(char *buffer, FILE *input_file) {
    do {
        fgets(buffer, MAX_LINE_LENGTH, input_file);
    } while (buffer[0] == '\n' || buffer[0] == '#');
}

void read_input_file(char *input_file) {
    FILE *image_file = fopen(input_file, "r");
    if (image_file == NULL) {
        perror("Error while here reading input file");
        exit(EXIT_FAILURE);
    }

    char buffer[MAX_LINE_LENGTH + 1];
    read_line(buffer, image_file);  // skip P2

    read_line(buffer, image_file);  // get width and height
    width = atoi(strtok(buffer, " \t\r\n"));
    height = atoi(strtok(NULL, " \t\r\n"));

    image = calloc(height, sizeof(unsigned char *));
    for (int i = 0; i < height; i++) {
        image[i] = calloc(width, sizeof(unsigned char));
    }

    read_line(buffer, image_file);  // skip gray value

    char *pixel = NULL;

    char *line = NULL;
    size_t len = 0;

    for (int i = 0; i < height * width; i++) {  // read pixels values
        if (pixel == NULL) {
            if (getline(&line, &len, image_file) == -1 && errno != 0) {
                perror("Error while reading input_file");
                exit(1);
            }
            pixel = strtok(line, " \t\r\n");
        }

        image[i / width][i % width] = atoi(pixel);
        pixel = strtok(NULL, " \t\r\n");
    }
    fclose(image_file);
}

void write_to_output_file(char *output_file) {
    FILE *negative_file = fopen(output_file, "w");
    if (negative_file == NULL) {
        perror("Error while reading output file");
        exit(EXIT_FAILURE);
    }

    fprintf(negative_file, "P2\n");
    fprintf(negative_file, "# %s\n", output_file);
    fprintf(negative_file, "%d %d\n", width, height);
    fprintf(negative_file, "255\n");

    int row_length = 0;

    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            char *pixel = calloc(4, sizeof(char));
            sprintf(pixel, "%d", negative[row][col]);
            if (row_length + strlen(pixel) == 69) {
                fprintf(negative_file, "%s\n", pixel);
                row_length = 0;
            } else if (row_length + strlen(pixel) > 69) {
                fprintf(negative_file, "\n");
                fprintf(negative_file, "%s ", pixel);
                row_length = strlen(pixel) + 1;
            } else {
                fprintf(negative_file, "%s ", pixel);
                row_length += strlen(pixel) + 1;
            }
            free(pixel);
        }
    }
    fclose(negative_file);
}

double* block_function(int *thread_index) {
    struct timespec start;
    clock_gettime(CLOCK_MONOTONIC, &start);

    int k = *thread_index;
    int x_min = k * ceil((double) width / threads_num);
    int x_max = min((k + 1) * ceil((double) width / threads_num) -  1, width - 1);
    for (int y = 0; y < height; y++) {
        for (int x = x_min; x <= x_max; x++) {
            negative[y][x] = 255 - image[y][x];
        }
    }
    double *time = calloc(1, sizeof(double));
    *time = get_time(&start);
    return time;
}

double* numbers_function(int *thread_index) {
    struct timespec start;
    clock_gettime(CLOCK_MONOTONIC, &start);

    int k = *thread_index;
    int min_gray_val = k * ceil((double) 256 / threads_num);
    int max_gray_val = min((k + 1) * ceil((double) 256 / threads_num) -  1, 255);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (image[y][x] >= min_gray_val && image[y][x] <= max_gray_val) {
                negative[y][x] = 255 - image[y][x];
            }
        }
    }
    double *time = calloc(1, sizeof(double));
    *time = get_time(&start);
    return time;
}

void free_tables() {
    for (int i = 0; i < height; i++) {
        free(image[i]);
    }
    for (int i = 0; i < threads_num; i++) {
        free(negative[i]);
    }
    free(image);
    free(negative);
}

int main(int argc, char** argv) {
    if (argc != 5) {
        fprintf(stderr, "Not a suitable number of program parameters. "
                        "Expected: ./main threads_num numbers/block input_file_name output_file_name\n");
        return 1;
    }

    threads_num = atoi(argv[1]);
    char *mode = argv[2];
    char *input_file = argv[3];
    char *output_file = argv[4];

//    used to create Times.txt
//    FILE *times = fopen("Times.txt", "a");
//    fprintf(times, "Number of threads: %d \t Mode: %s\n", threads_num, mode);


    read_input_file(input_file);

    struct timespec start;
    clock_gettime(CLOCK_MONOTONIC, &start);

    negative = calloc(height, sizeof(unsigned char *));
    for (int i = 0; i < height; i++) {
        negative[i] = calloc(width, sizeof(unsigned char));
    }

    double* (*func)(int *);

    if (!strcmp(mode, "block")) {
        func = block_function;
    } else if (!strcmp(mode, "numbers")) {
        func = numbers_function;
    }

    pthread_t *threads_ids = calloc(threads_num, sizeof(pthread_t));
    int *args = calloc(threads_num, sizeof(int));

    for (int i = 0; i < threads_num; i++) {
        args[i] = i;
        if (pthread_create(&threads_ids[i], NULL, (void * (*)(void *))func, &args[i]) == -1) {
            perror("Error while creating thread");
            return 1;
        }
    }

    for (int i = 0; i < threads_num; i++) {
        double *result;
        if (pthread_join(threads_ids[i], (void *)&result) == -1) {
            perror("Error while joining with a terminated thread");
            return 1;
        }
        printf("Thread %d \t %.3f microseconds\n", i + 1, *result);
//        fprintf(times, "Thread %d \t %.3f microseconds\n", i + 1, *result);  used to create Times.txt
        free(result);
    }

    double full_time = get_time(&start);
    printf("\nFull time %.3f microseconds\n==============================\n\n", full_time);
//    fprintf(times, "\nFull time %.3f microseconds\n==============================\n\n", full_time);  used to create Times.txt
//    fclose(times);

    write_to_output_file(output_file);

    free_tables();
    return 0;
}