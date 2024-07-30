/* File for the first pass of the assembler */
/* Date of creation - 18/7/2024 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "first_pass.h"
#include "errors.h"
#include "data_structures.h"
#include "lexer.h"


bool start_first_pass(char *file_name, Node **macro_head) {
    char buffer[BUFFER_SIZE];
    char cpy[MAX_CHAR_IN_LINE];
    char *am_file = NULL;
    char *token = NULL;

    FILE *fp_am = NULL;

    Node *label_head = NULL;
    labelNode *label_node = NULL;

    int IC = 0, DC = 0, line_number = 0;
    bool no_error = true;



    am_file = add_file_extension(file_name, AM_EXTENSION);
    fp_am = fopen(am_file, "r");
    /* In case the am file couldn't be open */
    if (fp_am == NULL) {
        free(am_file);
        generate_error(ERROR_COULDNT_OPEN_FILE, -1, "");
        return false;
    }
    free(am_file);
    fseek(fp_am, 0, SEEK_SET);

    /* Read the file content */
    while (fgets(buffer, BUFFER_SIZE, fp_am) != NULL) {
        buffer[strlen(buffer) - 1] = '\0';
        /* Checking if the line is too long */
        if (strlen(buffer) > MAX_CHAR_IN_LINE - 1) {
            generate_error(ERROR_LINE_TOO_LONG, line_number, "");
            no_error = false;
            continue;
        }
        strcpy(cpy, buffer);
        remove_leading_spaces(cpy);
        /* The line is a comment, therefore will be ignored */
        if(line_is_comment(cpy) == true){
            continue;
        }
        /* The line is a new line , therefore will be ignored */
        if(strcmp(cpy,"\n") == 0 || cpy[0] == '\0'){
            continue;
        }
        /* Locate ':' in the string */

        /* Locate '.' in the string */

    }
    fclose(fp_am);
    free_list(macro_head,MACRO,ALL);


    return true;
}