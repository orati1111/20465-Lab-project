/* 20465 - Lab - Project */
/* Date of Creation - 3/7/24 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"
#include "pre_assem.h"
#include "errors.h"

/* TODO: Add another argument to the function - the head of the macro node */
int start_pre_assem(char *file_name) {
    Node *macro_head = NULL;
    char *as_file = NULL;
    FILE *fp_as = NULL;

    /* Add the as extensions to the file name in order to open it. */
    as_file = add_file_extension(file_name, AS_EXTENSION);
    if(as_file == NULL)
        return false;
    fp_as = fopen(as_file, "r");

    /* In case the file doesn't exist/couldn't open it. */
    if (fp_as == NULL) {
        generate_error(ERROR_COULDNT_OPEN_FILE, -1);
        free(as_file);
        return false;
    }

    /* Add macros to the list */
    printf("STEP 1: add\n");
    if (add_macros_to_list(fp_as, &macro_head) == false) {
        free_list(&macro_head, MACRO, ALL);
        free(as_file);
        fclose(fp_as);
        return false;
    }

    /* Remove all the macro declaration and replace the macro calls with its content */
    printf("STEP 2: remove and replace\n");
    if (macro_expansion(fp_as,file_name,macro_head) == false) {
        free_list(&macro_head, MACRO, ALL);
        free(as_file);
        fclose(fp_as);
        return false;
    }
    printf("STEP 3: free all\n");
    fclose(fp_as);
    free_list(&macro_head, MACRO, ALL);
    free(as_file);
    return true;
}

int add_macros_to_list(FILE *fp, Node **head) {
    char buffer[MAX_CHAR_IN_LINE];
    char *macro_name, *macro_content, *temp_macro_name;
    int line_number = 0;
    macroNode *macro_node = NULL;
    Node *node = NULL, *temp = NULL;
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
            /* Checking if the given macro name is a reserved word or an invalid name. */
            if (is_macro_name_legal(macro_name) == false  || isalpha(macro_name[0]) == false) {
                generate_error(ERROR_ILLEGAL_MACRO_NAME, line_number);
                return false;
            }
            if (strlen(macro_name) > MAX_MACRO_LENGTH) {
                generate_error(ERROR_LONG_MACRO_NAME, line_number);
                return false;
            }

            temp = search_node(*head, macro_name, MACRO);
            /* Checking if the macro name already exists */
            if (temp != NULL) {
                generate_error(ERROR_MACRO_ALREADY_EXISTS, line_number);
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

            macro_node = create_macro_node(macro_name, macro_content);
            if (macro_node == NULL) {
                generate_error(ERROR_MALLOC_FAILED, -1);
                return false;
            }
            /* Since we are duplicating the strings, they should be freed. */
            free(macro_name);
            free(macro_content);
            node = create_node(macro_node, sizeof(macroNode), MACRO);
            free(macro_node);
            if (node != NULL) {
                add_node(head, node);
                print_macr_list(*head);
            } else {
                free_list(head, MACRO, ALL);
                generate_error(ERROR_COULDNT_CREATE_MACRO_NODE, line_number);
                return false;
            }
        }
    }

    return true;

}

char *save_macro_content(FILE *fp, fpos_t *pos, int *line_number) {
    char buffer[MAX_CHAR_IN_LINE];
    char *content = (char *) malloc(INITIAL_CONTENT_SIZE * sizeof(char));
    char *temp = NULL;
    size_t size = INITIAL_CONTENT_SIZE;
    size_t content_actual_size = 0;
    size_t current_line_size = 0;
    if (content == NULL) {
        generate_error(ERROR_MALLOC_FAILED, -1);
        return NULL;
    }
    while (fgets(buffer, MAX_CHAR_IN_LINE, fp) != NULL ){
        /* Checking if endmacr is a substring */
        if(strstr(buffer,"endmacr") != NULL)
            break;
        current_line_size = strlen(buffer);
        (*line_number)++;
        /* Checking if adding the new line exceeds the current size of the content */
        if(content_actual_size + current_line_size + 1 > size){
            size *= 2;
            temp = (char *) realloc(content,size*sizeof(char ));
            /* Reallocation failed */
            if(temp == NULL){
                generate_error(ERROR_REALLOC_FAILED, -1);
                free(content);
                return NULL;
            }
            content = temp;
        }
        strcpy(content+content_actual_size,buffer);
        content_actual_size += current_line_size;
    }
    if(size == content_actual_size){
        temp = (char *) realloc(content,(size+1)*sizeof(char ));
        if(temp == NULL)
            temp = content;
    }
    content[content_actual_size] = '\0';
    fsetpos(fp,pos);
    return content;
}



