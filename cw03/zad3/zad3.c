//
// Created by gcyganek on 22.03.2021.
//

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

char* create_path(char* path1, char* path2) {
    char* path = malloc(sizeof(char) * (strlen(path1) + strlen(path2) + 2));
    sprintf(path, "%s/%s", path1, path2);
    return path;
}

void scan_directory(char* path, int depth, int max_depth, char* substring) {
    if(depth > max_depth) return;
    DIR* dir = opendir(path);
    if(dir == NULL) {
        fprintf(stderr, "Error while opening directory %s: %s\n", path, strerror(errno));
        return;
    }

    struct dirent* ent;
    struct stat file_stat;
    while((ent = readdir(dir)) != NULL) {
        char* file_path = create_path(path, ent->d_name);
        if(lstat(create_path(path, ent->d_name), &file_stat) == -1) {
            fprintf(stderr, "Error while using lstat with %s file: %s", ent->d_name, strerror(errno));
            free(file_path);
            return;
        }

        if(S_ISREG(file_stat.st_mode)) {
            if(strstr(ent->d_name, substring)) {
                int file_name_length = strlen(ent->d_name);
                if(file_name_length > 4) {
                    const char *last_four = &ent->d_name[file_name_length - 4];
                    if(!strcmp(last_four, ".txt")) {
                       printf("%s\n", ent->d_name);
                    }
                }
            }
        }
        else if(S_ISDIR(file_stat.st_mode)) {
            if(!strcmp(ent->d_name, ".")) continue;
            if(!strcmp(ent->d_name, "..")) continue;
            scan_directory(file_path, depth + 1, max_depth, substring);
        }

        free(file_path);
    }

    closedir(dir);
}

int main(int argc, char** argv) {
    if(argc != 4) {
        fprintf(stderr, "Wrong number of arguments\n");
        return 1;
    }

    long max_depth = strtol(argv[3], NULL, 10);
    if(max_depth < 0) {
        fprintf(stderr, "Wrong argument value, depth of search should not be lower than 0, %ld given\n", max_depth);
        return 1;
    }

    scan_directory(argv[1], 0, max_depth, argv[2]);

    return 0;
}