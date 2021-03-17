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
    char* buffer = calloc(64, sizeof(char));

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

void write_to_file(int file1, int file2, char* to_change, char* change_result) {
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

                if(write(file2, before, index) == -1) {
                    fprintf(stderr, "Error while writing to file: %s", strerror(errno));
                    exit(1);
                }
                free(before);
            }

            if(write(file2, change_result, strlen(change_result)) == -1) {
                fprintf(stderr, "Error while writing to file: %s", strerror(errno));
                exit(1);
            }
            buffer += index + strlen(to_change);
            tmp_buffer += strlen(to_change);
            index = 0;
        }

        strcat(buffer, "\n");
        if(write(file2, buffer, strlen(buffer)) == -1) {
            fprintf(stderr, "Error while writing to file: %s", strerror(errno));
            exit(1);
        }
    }
    return;
}

int main(int argc, char** argv) {
    if(argc != 5) {
        fprintf(stderr, "Wrong number of arguments given\n");
        return 1;
    }

    char* file_name1 = argv[1];
    int file1 = open(file_name1, O_RDONLY);
    if(file1 == -1) {
        fprintf(stderr, "Error while reading file %s: %s\n", file_name1, strerror(errno));
        return 1;
    }
    char* file_name2 = argv[2];
    int file2 = open(file_name2, O_WRONLY|O_CREAT, S_IRUSR | S_IWUSR);
    if(file2 == -1) {
        fprintf(stderr, "Error while reading file %s: %s\n", file_name2, strerror(errno));
        return 1;
    }

    char* to_change = argv[3];
    char* change_result = argv[4];
    if(strlen(to_change) > 255 || strlen(change_result) > 255) {
        fprintf(stderr, "Given strings are too long");
    }

    write_to_file(file1, file2, to_change, change_result);
    return 0;
}