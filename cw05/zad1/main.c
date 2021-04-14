//
// Created by gcyganek on 12.04.2021.
//

#define  _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <assert.h>

#define MAX_COMMAND_ARGS 5
#define MAX_NUM_OF_COMMANDS 10  // ex. skladnik1 = cat /etc/passwd | grep sys is an ingredient with 2 commands, each with 2 args
#define MAX_NUM_OF_INGREDIENTS 10
#define MAX_INGREDIENTS_IN_LINE 10

typedef struct {
    char** args;
    int args_num;
    int max_arg_len : MAX_COMMAND_ARGS;
} Command;

typedef struct {
    Command** commands;
    int commands_num;
} Ingredient;

typedef struct {
    Ingredient** ingredients;
    int ingredients_num;
} Table;

Table* create_table() {
    Table* table = calloc(1, sizeof(Table));
    table->ingredients_num = MAX_NUM_OF_INGREDIENTS;
    table->ingredients = calloc(MAX_NUM_OF_INGREDIENTS, sizeof(Ingredient*));
    return table;
}

Ingredient* create_ingredient() {
    Ingredient* ingredient = calloc(1, sizeof(Ingredient));
    ingredient->commands = calloc(MAX_NUM_OF_COMMANDS, sizeof(Command*));
    return ingredient;
}

Command* create_command() {
    Command* command = calloc(1, sizeof(Command));
    command->args = calloc(MAX_COMMAND_ARGS, sizeof(char*));
    for (int i = 0; i < command->max_arg_len; i++) {
        command->args[i] = NULL;
    }
    return command;
}

void add_command_to_ingredient(char* command_data, Ingredient* ingredient, int command_index) {
    Command* command = create_command();
    char* pom = strtok(command_data, " ");
    command->args[0] = calloc(strlen(pom) + 1, sizeof(char));
    strcpy(command->args[0], pom);
    command->max_arg_len = strlen(command->args[0]);

    int arg_num = 1;
    while((pom = strtok(NULL, " ")) != NULL) {
        if (arg_num >= MAX_COMMAND_ARGS) break;
        command->args[arg_num] = calloc(strlen(pom) + 1, sizeof(char));
        if (strlen(command->args[arg_num]) > command->max_arg_len) {
            command->max_arg_len = strlen(command->args[arg_num]);
        }
        strcpy(command->args[arg_num++], pom);
    }

    command->args_num = arg_num;
    ingredient->commands[command_index] = command;
}

void create_ingredient_from_line(char* line, Table* table, int line_index) {
    Ingredient* ingredient = create_ingredient();

    char* save;
    char* command = strtok_r(line, "=", &save);

    int command_index = 0;
    while((command = strtok_r(NULL, "|", &save)) != NULL) {
        if (command_index >= MAX_NUM_OF_COMMANDS) break;
        char* pom = calloc(strlen(command) + 1, sizeof(char));
        pom = strdup(command);
        add_command_to_ingredient(pom, ingredient, command_index);
        command_index++;
        free(pom);
    }

    ingredient->commands_num = command_index;
    table->ingredients[line_index] = ingredient;
}

void close_table(Table* table) {
    Ingredient* ingredient;
    Command* command;
    for (int i = 0; i < table->ingredients_num && table->ingredients[i] != NULL; i++) {
        ingredient = table->ingredients[i];
        for (int j = 0; j < ingredient->commands_num; j++) {
            command = ingredient->commands[j];
            for (int k = 0; k < command->args_num && command->args[k] != NULL; k++) {
                free(command->args[k]);
            }
            free(command);
        }
        free(ingredient);
    }
    free(table);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Name of the text file with commands must be given.\n");
        return 1;
    }

    char* file_name = argv[1];
    FILE* commands_file = fopen(file_name, "r");
    if (commands_file == NULL) {
        fprintf(stderr, "Wrong commands file name.\n");
        return 1;
    }

    Table* table = create_table();

    char* line = NULL;
    size_t line_size = 0;
    int line_index = 0;
    char* ingredient_name = NULL;
    int commands_in_line = 0;
    Ingredient* ingredient;
    Command* command;

    int current[2], prev[2];

    int ingredients[MAX_INGREDIENTS_IN_LINE][1];
    int ingredients_index;

    while (getline(&line, &line_size, commands_file) != -1) {
        if (strlen(line) < 5) continue;  //ignoring the break line
        line = strtok(line, "\n");
        if (strchr(line, '=') != NULL) {  // reading components with commands
            create_ingredient_from_line(line, table, line_index);
        }

        else {  // executing ingredients
            ingredients_index = 0;
            ingredient_name = strtok(line, "|");
            while (ingredient_name != NULL) {
                int len = strlen(ingredient_name);
                while (ingredient_name[len - 1] == ' ') {
                    len -= 1;
                }
                int ingredient_index = ingredient_name[len - 1] - '0';
                ingredients[ingredients_index][0] = ingredient_index;
                ingredients_index++;

                ingredient_name = strtok(NULL, "|");
            }

            int j;
            for (int i = 0; i < ingredients_index; i++) {
                if (ingredients[i][0] == 0) {
                    ingredient = table->ingredients[9];
                } else {
                    ingredient = table->ingredients[ingredients[i][0] - 1];
                }

                for (j = 0; j < ingredient->commands_num && ingredient->commands[j] != NULL; j++) {
                    if (i == (ingredients_index - 1) && j == (ingredient->commands_num - 1)) break;
                    command = ingredient->commands[j];

                    pipe(current);

                    pid_t pid = fork();
                    if (pid == 0) {
                        if (i != 0 || j != 0) {
                            dup2(prev[0], STDIN_FILENO);
                            close(prev[1]);
                        }
                        dup2(current[1], STDOUT_FILENO);
                        if (execvp(command->args[0], command->args) == -1) {
                            exit(1);
                        }
                    }

                    close(current[1]);

                    prev[0] = current[0];
                    prev[1] = current[1];

                    commands_in_line++;
                }
            }

            pipe(current);
            pid_t pid = fork();
            command = ingredient->commands[j];

            if (pid == 0) {
                close(prev[1]);
                dup2(prev[0], STDIN_FILENO);

                if (execvp(command->args[0], command->args) == -1) {
                    exit(1);
                }
            }
            commands_in_line++;

            for (int i = 0; i < commands_in_line; i++) {
                wait(NULL);
            }

            commands_in_line = 0;
            printf("\n");
        }
        line_index++;
    }

    fclose(commands_file);
    close_table(table);
    return 0;
}