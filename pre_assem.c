/* 20465 - Lab - Project */
/* Date of Creation - 3/7/24 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "pre_assem.h"

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
        /* TODO: generate error - couldn't find file */
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

        }
    }
    return true;

}

