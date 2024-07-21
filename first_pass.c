/* File for the first pass of the assembler */
/* Date of creation - 18/7/2024 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "first_pass.h"
#include "errors.h"
#include "data_structures.h"


bool start_first_pass(char *file_name) {
    FILE *fp_am;
    char *am_file;
    int IC = 0, DC = 0;
    bool error_found = false;
    char buffer[MAX_CHAR_IN_LINE];
    char cpy[MAX_CHAR_IN_LINE];

    table *extern_table;
    table *entry_table;

    am_file = add_file_extension(file_name, AM_EXTENSION);
    fp_am = fopen(am_file, "r");
    /* In case the am file couldn't be open */
    if (fp_am == NULL) {
        free(am_file);
        generate_error(ERROR_COULDNT_OPEN_FILE, -1);
        return false;
    }


    fseek(fp_am, 0, SEEK_SET);
    /* Read the file content */
    while(fgets(buffer,MAX_CHAR_IN_LINE,fp_am) != NULL){
        /* The line is a comment */
        if(buffer[0] == ';')
            continue;
        /* Looking for .entry and .extern */
        /* Checking if there is a '.' in the line , NULL if there isn't */
        if(strchr(buffer,'.')){
            /* Looking for the substring .entry or .extern */
            if(strstr(buffer,".extern") || strstr(buffer,".entry")){

            }
        }
    }


    return true;
}