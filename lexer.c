/* File for the lexer */
/* Date of creation - 28/7/2024 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lexer.h"
#include "utils.h"
#include "errors.h"


bool line_is_comment(char *line) {
    return line[0] == ';';
}

/* TODO: add IC or DC idk */
bool construct_extern_entry(char *line, Node **macro_head, Node **label_head, int line_number, int *num_of_entries,
                            int *num_of_externs) {
    char *ptr = NULL;
    char *keyword = NULL;
    char *label_name = NULL;
    char *extra = NULL;
    labelNode *label_node = NULL;
    labelNode *found = NULL;
    Node *temp = NULL;
    Node *node = NULL;
    short address = 0;
    int error = NO_ERROR;
    labelType type;

    /* TODO: increment the number of entries or externs */
    if ((ptr = strstr(line, ".entry")) != NULL) {
        keyword = ".entry";
        type = ENTRY;
    } else if ((ptr = strstr(line, ".extern")) != NULL) {
        keyword = ".extern";
        type = EXTERN;
    }

    if (keyword == NULL) {
        generate_error(ERROR_UNKNOWN, line_number, line);
        return false;
    }
    ptr += strlen(keyword);
    label_name = strtok(ptr, " \n");
    remove_leading_spaces(label_name);
    extra = strtok(NULL, " \n");

    /* Checking if there are more than one label */
    /* TODO: fix that it prints only the first label in .entry MAIN MAIN2 it prints only .entry MAIN. */
    if (extra != NULL) {
        generate_error(ERROR_EXTRA_TEXT_AFTER_ENTRY_EXTERN, line_number, line);
        return false;
    }

    /* Checking if the name is invalid */
    if((error = validate_label_name(label_name,macro_head) != NO_ERROR)) {
        generate_error(error, line_number, line);
        return false;
    }

    /* TODO: search if the label already presents */
    /*
    if ((temp = search_node(*label_head, label_name, LABEL)) != NULL) {
        found = (labelNode *) temp->data;

    }
    */

    /* Creating a label node to store the label's information */
    label_node = create_label_node(label_name, address, type);
    if (label_node == NULL) {
        generate_error(ERROR_COULDNT_CREATE_NODE, -1, "");
        return false;
    }
    if ((node = create_node(label_node, sizeof(labelNode), LABEL)) == NULL) {
        generate_error(ERROR_COULDNT_CREATE_NODE, -1, "");
        free(label_node);
        free_list(label_head, LABEL, ALL);
        return false;
    }
    free(label_node);
    add_node(label_head, node);


    return true;


}

instrParts *construct_instruction(char *line, Node **macro_head, Node **label_head, int line_number) {
    instrParts * instruction = NULL;
    char * label_name = NULL;
    labelNode *label_node = NULL;
    int error = NO_ERROR;

    /* TODO: init instruction */
    instruction = malloc(sizeof(instrParts));
    if(instruction == NULL){
        generate_error(ERROR_MALLOC_FAILED,-1,"");
        exit(0);
    }
    init_instr_parts(instruction);
    printf("%d",instruction->type);

    label_name = line_is_label_decl(line);
    if(label_name != NULL){
        if((error = validate_label_name(label_name,macro_head) != NO_ERROR))
            generate_error(error,line_number,line);
    }
    free(label_name);
    free(instruction);
    return NULL;


}

char * line_is_label_decl(char * line){
    char cpy[MAX_CHAR_IN_LINE];
    char * token = NULL;
    strcpy(cpy, line);
    token = strtok(cpy,":");
    remove_leading_spaces(token);
    if(token != NULL)
        token = strdupli(token);
    return token;
}

/* TODO: check if there is a ':' in the middle of the line - not a label */
int validate_label_name(char *label_name, Node **macro_head) {

    /* Checking the length of the label */
    if (strlen(label_name) > MAX_MACRO_LENGTH) {
        return ERROR_LABEL_NAME_TOO_LONG;
    }
    /* Check if the name is a macro name , opcode or register.*/
    if (search_node(*macro_head, label_name, MACRO) != NULL || is_name_legal(label_name) == false) {
        return ERROR_ILLEGAL_LABEL_NAME;
    }
    return NO_ERROR;

}
