/* 20465 - Lab - Project */
/* Date of Creation - 3/7/24 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "pre_assem.h"
#include "errors.h"

int start_pre_assem(char *file_name) {
    Node *head = NULL;
    char *am_file;
    char *as_file;
    FILE *fp_as = NULL;
    FILE *fp_am = NULL;

    /* Add the as extension to the file name in order to open it. */
    as_file = add_file_extension(file_name, AS_EXTENSION);
    fp_as = fopen(as_file, "r");
    /* In case the file doesn't exist/couldn't open it. */
    if (fp_as == NULL) {
        generate_error(ERROR_COULDNT_OPEN_FILE,0);
        return false;
    }

    /*
    if (add_macros_to_list(fp_as,&head)==false){
        //free_node(head);
        fclose(fp_as);
        return false;
    }
     */
    add_macros_to_list(fp_as,&head);
    fclose(fp_as);

    return true;
}

int add_macros_to_list(FILE * fp, Node ** head){
    char buffer[MAX_CHAR_IN_LINE];
    char *macro_name,*macro_content;
    int line_number = 0;
    int current_macro_line;
    fpos_t pos;
    char * remaining;

    /* Reading the lines from the file*/
    while(fgets(buffer,MAX_CHAR_IN_LINE,fp)){
        line_number++;

        if(strcmp(strtok(buffer," "), "macr") == 0){
            current_macro_line = line_number;
            /* Getting the remainder of the macro declaration. */
            macro_name = strtok(NULL," \n");
            /* Checking if the macro name is empty */
            if(macro_name == NULL){
                generate_error(ERROR_INVALID_MACRO_DECL,line_number);
                return false;
            }

            /* Checking if the given macro name is a reserved word */
            /* TODO: remove the spaces if there are any in the macro name and then check for reserved words */
            if(is_macro_name_legal(macro_name) == true){
                generate_error(ERROR_ILLEGAL_MACRO_NAME,line_number);
                return false;
            }
            remaining = strtok(NULL,"\n");
            /* Checking if there is extra text after the macro declaration. */
            if (remaining != NULL){
                generate_error(ERROR_EXTRA_TEXT_AFTER_DECL,line_number);
                return false;
            }

            /* All is clear to continue saving the macro into the linked list */
            fgetpos(fp, &pos);
            macro_content = save_macro_content(fp,&pos,&line_number);



        }
    }
    return true;

}

char * save_macro_content(FILE * fp, fpos_t * pos, int * line_number){

}

