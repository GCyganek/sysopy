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

static char extensions[7][5] = {
        ".txt", ".h", ".c", ".json", ".java", ".py", ".cpp"
};

char* create_path(char* path1, char* path2) {
    char* path = malloc(sizeof(char) * (strlen(path1) + strlen(path2) + 2));
    sprintf(path, "%s/%s", path1, path2);
    return path;
}

char check_file_extension(char* file) {

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
    size_t size = 256;
    char* scan_result = malloc(size * sizeof(char));
    if(scan_result == NULL) {
        fprintf(stderr, "Error while allocating memory to %s: %s\n", scan_result, strerror(errno));
        closedir(dir);
        exit(1);
    }

    int len = 0;

    while((ent = readdir(dir)) != NULL) {
        char* file_path = create_path(path, ent->d_name);

        if(lstat(create_path(path, ent->d_name), &file_stat) == -1) {
            fprintf(stderr, "Error while using lstat with %s file: %s", ent->d_name, strerror(errno));
            free(file_path);
            closedir(dir);
            exit(1);
        }

        else if(S_ISDIR(file_stat.st_mode)) { // check if dir
            if(!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, "..")) continue;
            pid_t child_pid;
            child_pid = fork();
            if(child_pid == 0) {
                scan_directory(file_path, depth + 1, max_depth, substring);
                exit(0);
            }
        }

        if(S_ISREG(file_stat.st_mode)) {  // check if text file
            if(strstr(ent->d_name, substring)) {
                int file_name_length = strlen(ent->d_name);
                if(file_name_length > 4) {
                    const char *last_four = &ent->d_name[file_name_length - 4];
                    if(!strcmp(last_four, ".txt")) {
                        while(len + strlen(file_path) > size) {
                            size *= 2;
                            if(realloc(scan_result, size) == NULL) {
                                fprintf(stderr, "Error while allocating memory: %s\n", strerror(errno));
                                closedir(dir);
                                exit(1);
                            }
                        }
                        if(snprintf(scan_result + len, strlen(ent->d_name) + 2, "%s\n", ent->d_name) < 0) {
                            fprintf(stderr, "Error while writing to char array: %s", strerror(errno));
                            closedir(dir);
                            free(file_path);
                            exit(1);
                        }
                        len += strlen(scan_result);
                    }
                }
            }
        }

        free(file_path);
    }

    closedir(dir);

    while (waitpid(-1, NULL, 0) != -1) {
    }

    printf("directory:%s    pid:%d\n", path, getpid());
    printf("%s\n", scan_result);

    free(scan_result);
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