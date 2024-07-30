/* File for the lexer */
/* Date of creation - 28/7/2024 */

#include <stdio.h>
#include <string.h>
#include "lexer.h"
#include "utils.h"
#include "errors.h"


bool line_is_comment(char * line){
    return line[0] == ';';
}

bool handle_label_decl(char * line, Node ** macro_head,int line_number){
    char cpy[MAX_CHAR_IN_LINE];
    char * token = NULL;

    strcpy(cpy,line);
    token = strtok(cpy,":");
    token = remove_spaces(token);

    /* Checking the length of the label */
    if(strlen(token) > MAX_MACRO_LENGTH){
        generate_error(ERROR_LABEL_NAME_TOO_LONG, line_number, line);
        return false;
    }
    /* Check if the name is a macro name , opcode or register.*/
    if(search_node(*macro_head,token,MACRO) != NULL || is_name_legal(token)) {
        generate_error(ERROR_ILLEGAL_LABEL_NAME, line_number, line);
        return false;
    }
    return true;

}

bool tokenize(char * line, Node ** macro_head, int line_number){
    char cpy[MAX_CHAR_IN_LINE];
    char * token = NULL;

    strcpy(cpy,line);

    /* Tokenizing the string */
    return true;

}