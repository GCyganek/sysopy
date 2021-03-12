//
// Created by gcyganek on 09.03.2021.
//

#ifndef LIBRARY_H
#define LIBRARY_H

#define  _POSIX_C_SOURCE 200809L

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct {
    char** rows;
    int size;
} Block;

typedef struct {
    Block** blocks;
    int size;
} Table;

Table* create_table(int size);
Block* create_block(int size);
void write_files_to_tmp(char* file1, char* file2);
int create_block_from_tmp(Table* table);
void merge_file_sequence(Table* table, char** file_sequence);
int rows_in_block_count(Table* table, int block_id);
void delete_row(Table* table, int block_id, int row_id);
void delete_block(Table* table, int block_id);
void read_table(Table* table);
void free_table(Table* table);

#endif //LIBRARY_H
