/* File for the first pass of the assembler */
/* Date of creation - 18/7/2024 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "first_pass.h"
#include "errors.h"
#include "data_structures.h"
#include "lexer.h"
#include "encode.h"


bool start_first_pass(char *file_name, Node **macro_head, Node **label_head, codeWord memory[MAX_MEMORY_SIZE],
                      Node **unknown_label_head) {
    char buffer[BUFFER_SIZE];
    char cpy[MAX_CHAR_IN_LINE];
    int memory_length = MAX_MEMORY_SIZE - 1;
    int memory_ic_index = 0;
    int memory_dc_index = memory_length;
    codeWord code;
    char *am_file = NULL;

    int i;

    FILE *fp_am = NULL;

    instrParts *instruction = NULL;
    commandParts *command = NULL;

    int line_number = 0, num_of_entries = 0, num_of_externs = 0;
    unsigned short IC = 0, DC = 0;
    bool no_error = true;


    am_file = add_file_extension(file_name, AM_EXTENSION);
    fp_am = fopen(am_file, "r");
    /* In case the am file couldn't be open */
    if (fp_am == NULL) {
        cleanup("s", am_file);
        generate_error(ERROR_COULDNT_OPEN_FILE, -1, "");
        return false;
    }
    cleanup("s", am_file);
    fseek(fp_am, 0, SEEK_SET);

    /* Initializing memory */
    init_memory(memory);

    /* Read the file content */
    /* MAX_MEMORY_SIZE - OFFSET -> The memory size minus the 100 spaces that are not allowed to be used at the start */
    while ((fgets(buffer, BUFFER_SIZE, fp_am) != NULL) && (IC + DC < MAX_MEMORY_SIZE - OFFSET)) {
        line_number++;
        buffer[strlen(buffer)] = '\0';
        /* Checking if the line is too long */
        if (strlen(buffer) > MAX_CHAR_IN_LINE - 1) {
            generate_error(ERROR_LINE_TOO_LONG, line_number, "");
            no_error = false;
            continue;
        }
        strcpy(cpy, buffer);
        remove_leading_spaces(cpy);
        /* The line is a comment, therefore will be ignored */
        if (line_is_comment(cpy) == true) {
            continue;
        }
        /* The line is a new line , therefore will be ignored */
        if (strcmp(cpy, "\n") == 0 || cpy[0] == '\0') {
            continue;
        }

        /* Locate '.entry' or '.extern' */
        if (strstr(cpy, ".entry") != NULL || strstr(cpy, ".extern") != NULL) {
            construct_extern_entry(cpy, macro_head, label_head, line_number, &num_of_entries, &num_of_externs);
            /* TODO: probably do something here */
        } else if (strstr(cpy, ".data") || strstr(cpy, ".string")) {
            instruction = construct_instruction(cpy, macro_head, label_head, line_number, DC);
            if (instruction == NULL) {
                no_error = false;
            } else {
                encode_instruction(instruction, memory, &memory_dc_index, &DC);
                cleanup("i", instruction);
            }

        }
            /* TODO: add that it s probably an error */

            /* Command */
        else {
            command = construct_command(cpy, macro_head, label_head, line_number, IC);
            if (command != NULL) {
                encode_command(command, memory, &memory_ic_index, &IC, label_head, unknown_label_head, cpy,
                               line_number);
                cleanup("c", command);
            }
        }
//        print_label_list(*label_head);
    }
    fclose(fp_am);
    free_list(macro_head, MACRO, ALL);
    free_list(label_head, LABEL, ALL);
//    for(i = 0; i < memory_ic_index; i++){
//        print_bits(memory[i]);
//    }
//    for(i = memory_length ; i >= memory_dc_index ; i--){
//        print_bits(memory[i]);
//    }


    return no_error;
}