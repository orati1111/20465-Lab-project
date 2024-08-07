/* Utils source file */
/* Date of creation - 3/7/24 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>
#include "utils.h"
#include "errors.h"

const char *op_names[] = {"mov", "cmp", "add",
                          "sub", "lea", "clr",
                          "not", "inc", "dec",
                          "jmp", "bne", "red",
                          "prn", "jsr", "rts",
                          "stop"};

const char *registers[] = {"r0", "r1", "r2",
                           "r3", "r4", "r5",
                           "r6", "r7", "PSW"};


char *add_file_extension(char *base_file_name, char *extension) {
    size_t base_name_length = strlen(base_file_name);
    size_t extension_length = strlen(extension);
    size_t finalized_length = base_name_length + extension_length;
    char *finalized_file_name = NULL;

    /* Malloc memory for the file name with the extension */
    finalized_file_name = (char *) malloc((finalized_length + 1) * sizeof(char));
    if (finalized_file_name == NULL) {
        generate_error(ERROR_MALLOC_FAILED, -1,"");
        free(finalized_file_name);
        return NULL;
    }
    /* Copying the content to add the extension */
    strcpy(finalized_file_name, base_file_name);
    strcat(finalized_file_name, extension);
    finalized_file_name[finalized_length] = '\0';
    return finalized_file_name;

}

bool is_name_legal(char *name) {
    /* Name as the macro declaration */
    if (strcmp(name, "macr") == 0 || strcmp(name, "endmacr") == 0)
        return false;
    return (is_name_op_name(name) == -1) && !is_name_register(name) && isalpha(name[0]);

}
short is_name_op_name(char * name){
    short index;
    for (index = 0; index < NUM_OF_OPS; index++) {
        if (strcmp(op_names[index], name) == 0)
            return index;
    }
    return -1;
}

bool is_name_register(char * name){
    int index;
    for (index = 0; index < NUM_OF_REGISTERS; index++) {
        if (strcmp(registers[index], name) == 0)
            return true;
    }
    return false;
}

bool is_name_alphanumeric(char * name){
    char * ptr = NULL;
    ptr = name;
    while(*ptr != '\0'){
        if(!isalnum(*ptr))
            return false;
        ptr++;
    }
    return true;
}

char *strdupli(char *original) {
    char *cpy = NULL;
    if(original == NULL)
        return NULL;
    cpy = (char *) malloc(strlen(original) + 1);
    if (cpy == NULL)
        return NULL;
    strcpy(cpy, original);
    return cpy;
}

char *remove_spaces(const char *str) {
    size_t length;
    int i, j;
    char *result = NULL;
    j = 0;

    if(str == NULL)
        return NULL;
    length = strlen(str);
    result = malloc(length + 1);
    /* Copying only the chars that aren't spaces to the new string */
    for (i = 0; i < length; i++) {
        if (!isspace(str[i]))
            result[j++] = str[i];
    }
    result[j] = '\0';
    return result;
}

void remove_leading_spaces(char * str){
    int i = 0, j=0;

    if(str == NULL)
        return;

    while(str[i] && isspace(str[i]))
        i++;

    while(str[i])
        str[j++] = str[i++];

    str[j] = '\0';
}

void remove_trailing_spaces(char * str){
    size_t length = strlen(str);
    while(length > 0 && (str[length-1] == ' ' || str[length-1] == '\t' || str[length-1] == '\n')){
        str[length-1] = '\0';
        length--;
    }
}

bool macro_expansion(FILE *fp_as, char *original_file_name, Node *head) {
    char buffer[BUFFER_SIZE];
    char cpy[BUFFER_SIZE];
    FILE *fp_am = NULL;
    char *am_file = NULL;
    char *token = NULL;
    void *node_data = NULL;
    Node *temp = NULL;
    macroNode *node = NULL;
    bool in_macr_decl = false;

    /* Setting the position of the as file to the start */
    fseek(fp_as, 0, SEEK_SET);
    am_file = add_file_extension(original_file_name, AM_EXTENSION);
    /* Couldn't create the file name */
    if (am_file == NULL) {
        return false;
    }
    fp_am = fopen(am_file, "w");
    /* Couldn't open the file */
    if (fp_am == NULL) {
        generate_error(ERROR_COULDNT_WRITE_AM_FILE, -1,"");
        remove(am_file);
        return false;
    }
    /* Reading the file */
    while (fgets(buffer, BUFFER_SIZE, fp_as)) {
        strcpy(cpy, buffer);
        token = strtok(cpy," \n");
        if(token == NULL || strcmp(token,"") == 0)
            continue;
        /* Looking for the start of macro declaration */
        if (strncmp(token, "macr", 4) == 0 && strlen(token) == strlen("macr")) {
            in_macr_decl = true;
            continue;
        }
        if (strncmp(token, "endmacr", 7) == 0) {
            in_macr_decl = false;
            continue;
        }
        if (in_macr_decl == false) {
            /* Checking if it is a macro name */
            temp = search_node(head, token, MACRO);
            /* Found the macro */
            if (temp != NULL) {
                node_data = temp->data;
                node = (macroNode *) node_data;
                fprintf(fp_am, "%s", node->macr_content);
            }
                /* Other */
            else {
                strcpy(cpy,buffer);
                remove_leading_spaces(cpy);
                /* The line is a comment */
                if(cpy[0] == ';')
                    continue;
                fprintf(fp_am, "%s", cpy);
            }
        }

    }
    fclose(fp_am);
    free(am_file);
    return true;

}

bool check_extra_text(char *ptr, char * string){
    while(*ptr != '\0' && isspace(*ptr)){
        ptr++;
    }
    return *ptr != '\0';
}

void set_opcode_binary(codeWord *word, char opcode) {
    word->bits[1] |= (opcode << 3);
}

void cleanup(const char * order, ...){
    va_list args;
    char *str = NULL;
    int * numbers;
    FILE *fp = NULL;
    labelNode *label_node = NULL;
    instrParts *instruction = NULL;
    commandParts *command =  NULL;
    const char * ptr = NULL;

    va_start(args, order);
    for(ptr = order; *ptr != '\0'; ptr++){
        /* String */
        if(*ptr == 's'){
            str = va_arg(args, char*);
            if(str != NULL)
                free(str);
        }
        /* File pointer */
        if(*ptr == 'f'){
            fp = va_arg(args, FILE *);
            if (fp != NULL)
                fclose(fp);
        }
        if(*ptr == 'l'){
            label_node = va_arg(args, labelNode *);
            if(label_node != NULL){
                free(label_node);
            }
        }
        if(*ptr == 'i'){
            instruction = va_arg(args, instrParts *);
            if(instruction != NULL)
                free(instruction);
        }
        if(*ptr == 'n'){
            numbers = va_arg(args, int *);
            if(numbers != NULL)
                free(numbers);
        }
        if(*ptr == 'c'){
            command = va_arg(args, commandParts *);
            if(command != NULL)
                free(command);
        }
    }

    va_end(args);
}

bool check_multiple_commas(char * input){
    char * ptr = NULL;
    char * cpy = NULL;

    if(input == NULL)
        return false;

    cpy = strdupli(input);
    if(cpy == NULL){
        generate_error(ERROR_MALLOC_FAILED,-1, "");
        return false;
    }
    else{
        ptr = cpy;
        while(*ptr != '\0'){
            if(*ptr == ','){
                ptr++;
                if(*ptr == ','){
                    cleanup("s",cpy);
                    return true;
                }
            }
            else
                ptr++;
        }
    }
    cleanup("s",cpy);
    return false;

}

bool check_invalid_parentheses(char * input){
    int count = 0;

    /* Iterating and counting the amount if " in the string */
    while(*input != '\0'){
        if(*input == '"')
            count++;
        input++;
    }
    return count == 1;
}


bool is_whole_number(char * number){

    if (*number == '+' || *number == '-')
        number++;

    while(*number != '\0'){
        if(isdigit(*number) == 0)
            return false;
        number++;
    }
    return true;
}
