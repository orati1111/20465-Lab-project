/* File for the first pass of the assembler */
/* Date of creation - 18/7/2024 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "first_pass.h"
#include "errors.h"
#include "data_structures.h"
#include "lexer.h"


bool start_first_pass(char *file_name, Node **macro_head, Node ** label_head) {
    char buffer[BUFFER_SIZE];
    char cpy[MAX_CHAR_IN_LINE];
    char *am_file = NULL;
    char *token = NULL;

    int i;

    FILE *fp_am = NULL;

    labelNode *label_node = NULL;

    instrParts *instruction = NULL;
    commandParts *command = NULL;

    int IC = 0, DC = 0, line_number = 0, num_of_entries = 0, num_of_externs = 0;
    bool no_error = true;


    am_file = add_file_extension(file_name, AM_EXTENSION);
    fp_am = fopen(am_file, "r");
    /* In case the am file couldn't be open */
    if (fp_am == NULL) {
        cleanup("s",am_file);
        generate_error(ERROR_COULDNT_OPEN_FILE, -1, "");
        return false;
    }
    cleanup("s",am_file);
    fseek(fp_am, 0, SEEK_SET);

    /* Read the file content */
    /* TODO: add a memory overflow IC DC break condition */
    while (fgets(buffer, BUFFER_SIZE, fp_am) != NULL) {
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

        /* Locate '.' in the string */
        if(strchr(cpy,'.') != NULL){
            /* Locate '.entry' or '.extern' */
            if (strstr(cpy,".entry") != NULL || strstr(cpy, ".extern") != NULL){
                construct_extern_entry(cpy, macro_head, label_head, line_number,&num_of_entries, &num_of_externs);

            }
            if(strstr(cpy,".data") || strstr(cpy, ".string")){
                instruction = construct_instruction(cpy,macro_head,label_head,line_number);
                if(instruction == NULL){
                    no_error = false;
                }
                else{
                    /* TODO: delete this */
                    if(instruction->type == DATA) {
                        for (i = 0; i < instruction->length.amount_of_numbers; i++) {
                        }
                        cleanup("ni",instruction->data.numbers,instruction);
                    }
                    else if(instruction->type == STRING){
                        cleanup("si",instruction->data.string,instruction);
                    }
                }

            }

        }
    }
    fclose(fp_am);
    free_list(macro_head, MACRO, ALL);
    free_list(label_head,LABEL,ALL);


    return no_error;
}