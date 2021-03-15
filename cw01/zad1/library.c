//
// Created by gcyganek on 09.03.2021.
//

#include "../zad2/library.h"

const char* TMP_FILE = "tmp";
int tmp_file_size = -1;
int merge_file_sequence_index = -1;

Table* create_table(int size) {
    if(size < 0) {
        printf("Wrong size value given: %d in operation create_table\n", size);
        return NULL;
    }
    Table* table = calloc(sizeof(Table), 1);
    table->size = size;
    table->blocks = calloc(sizeof(Block*), size);
    return table;
}

Block* create_block(int size) {
    if(size < 0) {
        printf("Wrong size value: %d in operation create_block\n", size);
        return NULL;
    }
    Block* block = calloc(sizeof(Block), 1);
    block->size = size;
    block->rows = calloc(sizeof(char*), size);
    return block;
}

void write_files_to_tmp(char* file1, char* file2) {
    int rows_count = 0;

    FILE *tmp_file = fopen(TMP_FILE, "w");
    FILE *f1 = fopen(file1, "r");
    if(f1 == NULL) {
        fclose(tmp_file);
        printf("\nError while loading file %s in operation write_files_to_tmp\n", file1);
        return;
    }
    FILE *f2 = fopen(file2, "r");
    if(f2 == NULL) {
        fclose(tmp_file);
        fclose(f1);
        printf("\nError while loading file %s in operation write_files_to_tmp\n", file2);
        return;
    }

    char *line1 = NULL, *line2 = NULL;
    size_t line_size = 0;

    while ((getline(&line1, &line_size, f1) != -1) && (getline(&line2, &line_size, f2) != -1)) {
        if (feof(f1)) {
            fprintf(tmp_file, "%s\n", line1);
            fprintf(tmp_file, "%s", line2);
        } else if (feof(f2)) {
            fprintf(tmp_file, "%s", line1);
            fprintf(tmp_file, "%s\n", line2);
        } else {
            fprintf(tmp_file, "%s", line1);
            fprintf(tmp_file, "%s", line2);
        }
        rows_count += 2;
    }
    while(getline(&line1, &line_size, f1) != -1) {
        if (feof(f1)) {
            fprintf(tmp_file, "%s\n", line1);
        } else {
            fprintf(tmp_file, "%s", line1);
        }
        rows_count += 1;
    }
    while(getline(&line2, &line_size, f2) != -1) {
        if (feof(f2)) {
            fprintf(tmp_file, "%s\n", line2);
        } else {
            fprintf(tmp_file, "%s", line2);
        }
        rows_count += 1;
    }

    fclose(f1);
    fclose(f2);
    fclose(tmp_file);

    tmp_file_size = rows_count;
}

int create_block_from_tmp(Table* table) {
    FILE* tmp_file = fopen(TMP_FILE, "r");
    if(tmp_file == NULL) {
        printf("\nError in create_block_from_tmp operation: tmp_file is invalid\n");
        return -1;
    }
    if(tmp_file_size == -1) {
        fclose(tmp_file);
        printf("\nError in create_block_from_tmp operation: tmp_file is invalid\n");
        return -1;
    }
    int row_index = 0;
    size_t line_size = 0;

    Block* block = create_block(tmp_file_size);

    while(getline(&block->rows[row_index], &line_size, tmp_file) != -1) {
        row_index += 1;
    }

    fclose(tmp_file);

    int table_index;
    if(merge_file_sequence_index == -1) {
        int size = table->size;
        int i = 0;
        while((i < size) && (table->blocks[i] != NULL)) {
            i++;
        }
        if(i >= size) {
            printf("\nError in create_block_from_tmp operation: index out of table range\n");
            return -1;
        } else {
            table->blocks[i] = block;
            table_index = i;
        }
    } else {
        table_index = merge_file_sequence_index;
        table->blocks[table_index] = block;
        merge_file_sequence_index = -1;
    }

    return table_index;
}

void merge_file_sequence(Table* table, char** file_sequence) {
    if(table == NULL) {
        printf("\nError in merge_file_sequence operation: No table created\n");
        return;
    }

    int size = table->size;
    char *file1, *file2;

    for(int i = 0; i < size; i++) {
        file1 = strtok(file_sequence[i], ":");
        file2 = strtok(NULL, "");
        write_files_to_tmp(file1, file2);

        merge_file_sequence_index = i;
        create_block_from_tmp(table);
    }
}

int rows_in_block_count(Table* table, int block_id) {
    if(table == NULL) {
        printf("\nError in rows_in_block_count operation: No table created\n");
        return -1;
    }
    if(block_id < 0 || block_id >= table->size) {
        printf("\nWrong block_id: %d given for rows_in_block_count operation\n", block_id);
        return -1;
    }

    Block* block = table->blocks[block_id];
    int size = block->size;

    int count = 0;
    for(int i = 0; i < size; i++) {
        if(block->rows[i] != NULL) count++;
    }

    return count;
}

void delete_row(Table* table, int block_id, int row_id) {
    if(table == NULL) {
        printf("\nError in delete_row operation: No table created\n");
        return;
    }
    if(block_id < 0 || block_id >= table->size) {
        printf("\nWrong block_id: %d given for delete_row operation\n", block_id);
        return;
    }
    if(table->blocks[block_id] == NULL) {
        printf("\nError in delete_row operation: Given block (block_id: %d) is NULL\n", block_id);
    }

    Block* block = table->blocks[block_id];

    if(row_id < 0 || row_id >= block->size) {
        printf("\nWrong row_id: %d given for delete_row operation\n", row_id);
        return;
    }
    if(block->rows[row_id] == NULL) {
        printf("\nError in delete_row operation: Given row (row_id: %d) is NULL\n", row_id);
    }

    free(block->rows[row_id]);
    block->rows[row_id] = NULL;
}

void delete_block(Table* table, int block_id) {
    if(table == NULL) {
        printf("\nError in delete_block operation: No table created\n");
        return;
    }
    if(block_id < 0 || block_id >= table->size) {
        printf("\nWrong block_id: %d given for delete_block operation\n", block_id);
        return;
    }
    if(table->blocks[block_id] == NULL) {
        printf("\nError in delete_block operation: Given block (block_id: %d) is NULL\n", block_id);
        return;
    }
    free(table->blocks[block_id]);
    table->blocks[block_id] = NULL;
}

void free_table(Table* table) {
    if(table == NULL) {
        printf("\nError in free_table operation: No table created\n");
        return;
    }

    int size = table->size;
    for(int i = 0; i < size; i++) {
        if (table->blocks[i] != NULL) {
            delete_block(table, i);
        }
    }

    free(table);
}

void read_table(Table* table) {
    if(table == NULL) {
        printf("\nError in read_table operation: No table created\n");
        return;
    }

    int tab_size = table->size;
    Block* block;
    int block_size;

    char* row;
    for(int i = 0; i < tab_size; i++) {
        block = table->blocks[i];
        if(block != NULL) {
            printf("\n      block nr %d:\n", i);
            block_size = block->size;
            for(int j = 0; j < block_size; j++) {
                row = block->rows[j];
                if(row != NULL) {
                    printf("row[%d]: %s", j, block->rows[j]);
                } else {
                    printf("row[%d]: NULL\n", j);
                }
            }
        }
    }
}