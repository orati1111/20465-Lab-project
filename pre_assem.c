/* 20465 - Lab - Project */
/* Date of Creation - 3/7/24 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "pre_assem.h"
#include "errors.h"

int start_pre_assem(char *file_name) {
    Node *macro_head = NULL;
    char *tmp_file;
    char *as_file;
    FILE *fp_as = NULL;

    /* Add the as extensions to the file name in order to open it. */
    as_file = add_file_extension(file_name, AS_EXTENSION);
    tmp_file = add_file_extension(file_name, TMP_EXTENSION);
    fp_as = fopen(as_file, "r");
    /* In case the file doesn't exist/couldn't open it. */
    if (fp_as == NULL) {
        generate_error(ERROR_COULDNT_OPEN_FILE, -1);
        free(as_file);
        free(tmp_file);
        return false;
    }
    /* Add macros to the list */
    if (add_macros_to_list(fp_as, &macro_head) == false) {
        free_node(macro_head);
        free(as_file);
        free(tmp_file);
        fclose(fp_as);
        return false;
    }
    /* Remove all the macro declaration and replace the macro calls with its content */
    if (remove_and_replace_macros(fp_as, tmp_file, file_name, &macro_head) == false) {
        free_node(macro_head);
        free(as_file);
        free(tmp_file);
        fclose(fp_as);
        return false;
    }

    fclose(fp_as);
    free_node(macro_head);
    free(as_file);
    free(tmp_file);
    remove(tmp_file);
    return true;
}

int add_macros_to_list(FILE *fp, Node **head) {
    char buffer[MAX_CHAR_IN_LINE];
    char *macro_name, *macro_content, *temp_macro_name;
    int line_number = 0;
    int found_macro = false;
    fpos_t pos;
    char *remaining;

    /* Reading the lines from the file*/
    while (fgets(buffer, MAX_CHAR_IN_LINE, fp)) {
        line_number++;
        if (strcmp(strtok(buffer, " "), "macr") == 0) {
            /* Getting the remainder of the macro declaration. */
            temp_macro_name = strtok(NULL, " \n");

            /* Checking if the macro name is empty */
            if (temp_macro_name == NULL) {
                generate_error(ERROR_INVALID_MACRO_DECL, line_number);
                return false;
            }

            /* Removing the spaces from the macro name (if there are any) */
            macro_name = remove_spaces(temp_macro_name);
            /* Checking if the given macro name is a reserved word. */
            if (is_macro_name_legal(macro_name) == false) {
                generate_error(ERROR_ILLEGAL_MACRO_NAME, line_number);
                return false;
            }
            if(strlen(macro_name) > MAX_MACRO_LENGTH){
                generate_error(ERROR_LONG_MACRO_NAME,line_number);
            }
            search_node(*head, macro_name, &found_macro);
            /* Checking if the macro name already exists */
            if (found_macro) {
                generate_error(ERROR_MACRO_ALREADY_EXISTS, line_number);
                found_macro = false;
                return false;
            }

            remaining = strtok(NULL, "\n");
            /* Checking if there is extra text after the macro declaration. */
            if (remaining != NULL) {
                generate_error(ERROR_EXTRA_TEXT_AFTER_MACRO_NAME, line_number);
                return false;
            }

            /* All is clear to continue saving the macro into the linked list */
            fgetpos(fp, &pos);
            macro_content = save_macro_content(fp, &pos, &line_number);
            if (macro_content == NULL)
                return false;

            fsetpos(fp, &pos);
            /* TODO: Create a node first and then add node*/
            add_node(head, macro_name, macro_content);
        }
    }

    return true;

}

char *save_macro_content(FILE *fp, fpos_t *pos, int *line_number) {
    char content[MAX_CHAR_IN_LINE];
    unsigned long long content_length = 0;
    char *temp = NULL;
    content[0] = '\0';

    /* Setting the position of the pointer in the file to the correct place */
    if (fsetpos(fp, pos) != 0) {
        generate_error(ERROR_COULDNT_SET_POS_POINTER, *line_number);
        return NULL;
    }
    /* While reading the content of the file, look for endmacr */
    while (fgets(content, MAX_CHAR_IN_LINE, fp) &&
           !(((strcmp(content, "endmacr\n")) != 0) ^ (strcmp(content, "endmacr") != 0))) {
        (*line_number)++;
        /* In case endmacr is a part of the string but there are extra text after it */
        if ((strstr(content, "endmacr") != NULL) && strlen(content) != strlen("endmacr ")) {
            generate_error(ERROR_EXTRA_TEXT_AFTER_ENDMACR, *line_number);
            return NULL;
        }
        /* Reaching endmacr - enlarge the content length to fit the size of the content */
        if (strcmp(content, "endmacr\n") != 0) {
            content_length += strlen(content);
        }

    }
    /* Copy the content of the macro */
    temp = copy_text(fp, pos, content_length);
    return temp;

}



