//
// Created by gcyganek on 13.04.2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <sys/file.h>

int main(int argc, char* argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Wrong number of arguments given. Expected: pipe_file, row_number,"
                        " text_file, buffer_size=N\n");
        return 1;
    }

    int fifo_file = open(argv[1], O_WRONLY);
    if (fifo_file == -1) {
        fprintf(stderr, "Error while opening pipe_file: %s\n", strerror(errno));
    }

    int text_file = open(argv[3], O_RDONLY);
    if (text_file == -1) {
        fprintf(stderr, "Error while opening text_file: %s\n", strerror(errno));
    }

    int row_number = strtol(argv[2], NULL, 10);
    int N = strtol(argv[4], NULL, 10);
    char* buffer = calloc(N + 1, sizeof(char));

    srand(time(NULL));

    int chars_read;
    while((chars_read = read(text_file, buffer, sizeof(char) * N)) > 0) {
        sleep(rand() % 2 + 1);
        char to_fifo[chars_read + 3];
        sprintf(to_fifo, "%d|%s", row_number, buffer);
        if (flock(fifo_file, LOCK_EX) == -1 ) {
            fprintf(stderr, "Error while using flock() in producer: %s", strerror(errno));
        }
        write(fifo_file, to_fifo, strlen(to_fifo));
        if (flock(fifo_file, LOCK_UN) == -1 ) {
            fprintf(stderr, "Error while using flock() in producer: %s", strerror(errno));
        }
    }
    free(buffer);
    close(fifo_file);
    return 0;
}