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
#include <unistd.h>
#include <sys/wait.h>

static const char extensions[][6] = {
        ".txt", ".h", ".c", ".json", ".java", ".csv", ".cpp"
};

char* create_path(char* path1, char* path2) {
    char* path = malloc(sizeof(char) * (strlen(path1) + strlen(path2) + 2));
    sprintf(path, "%s/%s", path1, path2);
    return path;
}

char check_file_extension(char* file, struct dirent* ent) {
    int file_name_length = strlen(ent->d_name);
    int extension_length;
    for(size_t i = 0; i < sizeof(extensions) / sizeof(extensions[0]); i++) {
        extension_length = strlen(extensions[i]);
        if(file_name_length > extension_length) {
            const char *end = &ent->d_name[file_name_length - extension_length];
            if(!strcmp(end, extensions[i])) {
                return 1;
            }
        }
    }
    return 0;
}

char check_is_text_file_with_substr(char* file, struct dirent* ent, struct stat* file_stat, char* substring) {
    if(S_ISREG(file_stat->st_mode)) {
        if(strstr(ent->d_name, substring)) {
            if(check_file_extension(file, ent)) {
                return 1;
            }
        }
    }
    return 0;
}

void scan_directory(char* path, int depth, int max_depth, char* substring) {
    if(depth > max_depth) return;
    DIR* dir = opendir(path);
    if(dir == NULL) {
        fprintf(stderr, "Error while opening directory %s: %s\n", path, strerror(errno));
        exit(1);
    }

    struct dirent* ent;
    struct stat file_stat;

    while((ent = readdir(dir)) != NULL) {
        char* file_path = create_path(path, ent->d_name);

        if(lstat(create_path(path, ent->d_name), &file_stat) == -1) {
            fprintf(stderr, "Error while using lstat with %s file: %s", ent->d_name, strerror(errno));
            free(file_path);
            closedir(dir);
            exit(1);
        }

        if((depth + 1 <= max_depth) && S_ISDIR(file_stat.st_mode)) { // check if dir
            if(!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, "..")) continue;
            pid_t child_pid;
            child_pid = fork();
            if(child_pid == 0) {
                scan_directory(file_path, depth + 1, max_depth, substring);
                exit(0);
            }
        }

        if(check_is_text_file_with_substr(file_path, ent, &file_stat, substring)) {
            printf("File containing the given substring found in directory:%s by process with pid:%d\t\t\t\t%s\n", path, getpid(), ent->d_name);
        }

        free(file_path);
    }

    closedir(dir);

    while (waitpid(-1, NULL, 0) != -1) {
    }

    exit(0);
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

    pid_t child_pid;
    child_pid = fork();
    if(child_pid == 0){
        scan_directory(argv[1], 0, max_depth, argv[2]);
    }

    while (waitpid(-1, NULL, 0) != -1) {
    }

    return 0;
}