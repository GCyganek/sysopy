//
// Created by gcyganek on 13.04.2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/file.h>

void flock_err() {
    fprintf(stderr, "Error while using flock() in consumer: %s\n", strerror(errno));
    exit(1);
}

void rewrite_modified_file(FILE* text_file, int modified_row, char* string_to_add) {
    rewind(text_file);

    int new_lines = 0;
    char c;
    char* buf = malloc(32 * sizeof(char));
    size_t length = 0;
    size_t size = 32;
    while ((c = fgetc(text_file)) != EOF) {
        if (length == size) {
            size *= 2;
            buf = realloc(buf, size);
            if (buf == NULL) {
                fprintf(stderr, "Error while reallocating buf in consumer: %s\n", strerror(errno));
                exit(1);
            }
        }
        if(c == '\n') new_lines++;
        buf[length++] = c;
    }

    if((new_lines + 1) >= modified_row) {  // modifying given row in file
        rewind(text_file);

        int line = 1;
        size_t pos;
        char added = 0;
        for (pos = 0; pos < length; pos++) {
            c = buf[pos];
            if (!added && (c == '\n' && line == modified_row)) {
                fputs(string_to_add, text_file);
                added = 1;
                fputc(c, text_file);
            }
            else if (!added && (line == modified_row && pos == length - 1)) {
                fputc(c, text_file);
                fputs(string_to_add, text_file);
            } else {
                fputc(c, text_file);
            }
            if (c == '\n') line++;
        }

        if (length == 0) {
            fputs(string_to_add, text_file);
        }
    }

    else {  // appending to the end of file
        char* to_add = malloc((strlen(string_to_add) + modified_row - new_lines) * sizeof(char));
        to_add[0] = '\n';
        new_lines++;
        while ((new_lines + 1) < modified_row) {
            to_add = strcat(to_add, "\n");
            new_lines++;
        }
        to_add = strcat(to_add, string_to_add);
        fputs(to_add, text_file);
    }

    free(buf);
    rewind(text_file);
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Wrong number of arguments given. Expected: pipe_file, text_file, buffer_size=N\n");
        return 1;
    }

    int fifo_file = open(argv[1], O_RDONLY);
    if (fifo_file == -1) {
        fprintf(stderr, "Error while opening pipe_file in consumer: %s\n", strerror(errno));
    }

    FILE* text_file = fopen(argv[2], "w+");
    if (text_file == NULL) {
        fprintf(stderr, "Error while opening text_file in consumer: %s\n", strerror(errno));
    }

    int text_file_descriptor = fileno(text_file);

    int N = strtol(argv[3], NULL, 10);
    char* buffer = calloc(N + 3, sizeof(char)); // N + 3 because we need space for row_number + "|" + N chars + '\0'
    char* to_text_file = calloc(N + 1, sizeof(char));

    int row_id;
    int chars_read;
    int lock, unlock;
    while((chars_read = read(fifo_file, buffer, (N + 2) * sizeof(char))) > 0) {
        row_id = atoi(&buffer[0]);
        to_text_file = buffer + 2;
        if ((lock = flock(text_file_descriptor, LOCK_EX)) < 0) flock_err();
        rewrite_modified_file(text_file, row_id, to_text_file);
        if ((unlock = flock(text_file_descriptor, LOCK_UN)) < 0) flock_err();
    }
    free(buffer);
    fclose(text_file);
    return 0;
}