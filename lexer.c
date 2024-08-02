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
    char *cpy = NULL;
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

    cpy = strdupli(line);
    if ((ptr = strstr(cpy, ".entry")) != NULL) {
        keyword = ".entry";
        type = ENTRY;
    } else if ((ptr = strstr(cpy, ".extern")) != NULL) {
        keyword = ".extern";
        type = EXTERN;
    }

    if (keyword == NULL) {
        generate_error(ERROR_UNKNOWN, line_number, line);
        cleanup("s", cpy);
        return false;
    }
    ptr += strlen(keyword);
    label_name = strtok(ptr, " \n");
    remove_leading_spaces(label_name);
    extra = strtok(NULL, " \n");

    /* Checking if there are more than one label */
    if (extra != NULL) {
        generate_error(ERROR_EXTRA_TEXT_AFTER_ENTRY_EXTERN, line_number, line);
        cleanup("s", cpy);
        return false;
    }

    /* Checking if the name is invalid */
    if ((error = validate_label_decl(label_name, macro_head, label_head, type)) != NO_ERROR) {
        generate_error(error, line_number, line);
        cleanup("s", cpy);
        return false;
    }

    /* Creating a label node to store the label's information */
    label_node = create_label_node(label_name, address, type);
    if (label_node == NULL) {
        generate_error(ERROR_COULDNT_CREATE_NODE, -1, "");
        cleanup("s", cpy);
        return false;
    }
    if ((node = create_node(label_node, sizeof(labelNode), LABEL)) == NULL) {
        generate_error(ERROR_COULDNT_CREATE_NODE, -1, "");
        cleanup("sl", cpy, label_node);
        free_list(label_head, LABEL, ALL);
        return false;
    }

    cleanup("sl", cpy, label_node);
    add_node(label_head, node);
    if (type == EXTERN)
        (*num_of_externs)++;
    else {
        (*num_of_entries)++;
    }


    return true;


}

instrParts *construct_instruction(char *line, Node **macro_head, Node **label_head, int line_number) {
    char *cpy = NULL;
    char *string = NULL;
    char *label_name = NULL;
    int *numbers = NULL;
    instrParts *instruction = NULL;
    labelNode *label_node = NULL;
    Node *temp = NULL;

    int error = NO_ERROR;
    short num_of_numbers = 0;

    instruction = malloc(sizeof(instrParts));
    if (instruction == NULL) {
        generate_error(ERROR_MALLOC_FAILED, -1, "");
        exit(0);
    }
    init_instr_parts(instruction);

    label_name = line_is_label_decl(line);
    if (label_name != NULL) {
        if ((error = validate_label_decl(label_name, macro_head, label_head, LOCAL)) != NO_ERROR) {
            generate_error(error, line_number, line);
            cleanup("i", instruction);
            return NULL;
        }
        /* TODO: change here when i understand the IC DC thing */
        label_node = create_label_node(label_name, 0, LOCAL);
        if (label_node == NULL) {
            generate_error(ERROR_COULDNT_CREATE_NODE, -1, "");
            cleanup("is", instruction, label_name);
            return NULL;
        }
        temp = create_node(label_node, sizeof(labelNode), LABEL);
        cleanup("ls", label_node, label_name);
        if (temp != NULL) {
            add_node(label_head, temp);
            instruction->label = temp;
        } else {
            cleanup("i", instruction);
            return NULL;
        }

    }

    /* Handling data instruction */
    if (strstr(line, ".data") != NULL) {
        instruction->type = DATA;
        cpy = strdupli(line);
        numbers = get_data(cpy, line_number, &num_of_numbers);
        if(numbers == NULL){
            cleanup("is",instruction, cpy);
            return NULL;
        }
        instruction->data.numbers = numbers;
        instruction->length.amount_of_numbers = num_of_numbers;


        /* Handling string instruction */
    } else if (strstr(line, ".string") != NULL) {
        instruction->type = STRING;
        cpy = strdupli(line);
        if (strchr(cpy, '"') == NULL || check_invalid_parentheses(cpy)) {
            generate_error(ERROR_INVALID_STRING_FORMAT, line_number, line);
            free_list(label_head, LABEL, ALL);
            cleanup("is", instruction, cpy);
            return NULL;
        }
        if ((string = get_string(cpy)) == NULL) {
            free_list(label_head, LABEL, ALL);
            cleanup("is", instruction, cpy);
            return NULL;
        }
        instruction->data.string = string;
        instruction->length.string_length = strlen(string) + 1;

    }

    cleanup("s", cpy);
    return instruction;


}

commandParts * construct_command(){

}

char *line_is_label_decl(char *line) {
    char cpy[MAX_CHAR_IN_LINE];
    char *token = NULL;
    strcpy(cpy, line);
    if(strchr(cpy,':') == NULL)
        return NULL;
    token = strtok(cpy, ":");
    remove_leading_spaces(token);
    if (token != NULL)
        token = strdupli(token);
    return token;
}

/* TODO: check if there is a ':' in the middle of the line - not a label */
int validate_label_decl(char *label_name, Node **macro_head, Node **label_head, labelType type) {
    Node *temp = NULL;
    labelNode *found = NULL;

    /* Checking the length of the label */
    if (strlen(label_name) > MAX_MACRO_LENGTH) {
        return ERROR_LABEL_NAME_TOO_LONG;
    }
    /* Check if the name is a macro name , opcode or register.*/
    if (search_node(*macro_head, label_name, MACRO) != NULL || is_name_legal(label_name) == false) {
        return ERROR_ILLEGAL_LABEL_NAME;
    }
    if ((temp = search_node(*label_head, label_name, LABEL)) != NULL) {
        found = (labelNode *) temp->data;
        /* Already existing label */
        if (found->label_type == LOCAL && type == LOCAL)
            return ERROR_LABEL_ALREADY_EXISTS;
        /* An attempt to declare a label as extern but its declared locally */
        if (found->label_type == LOCAL && type == EXTERN) {
            return ERROR_LOCAL_AS_EXTERN;
        }
        if (found->label_type == EXTERN && type == LOCAL) {
            return ERROR_EXTERN_AS_LOCAL;
        }
    }
    return NO_ERROR;

}

char *get_string(const char *line) {
    char *start_ptr = NULL;
    char *end_ptr = NULL;
    char *result = NULL;
    size_t size;

    /* Locate the first and last occurrence of " and set them into pointers */
    /* Assuming that there is " in the string */
    start_ptr = strchr(line, '"');
    end_ptr = strrchr(line, '"');

    /* Move the pointer past the first " */
    start_ptr++;

    size = end_ptr - start_ptr;
    result = malloc((size + 1) * sizeof(char));
    if (result == NULL) {
        generate_error(ERROR_MALLOC_FAILED, -1, "");
        return NULL;
    }
    strncpy(result, start_ptr, size);
    result[size] = '\0';
    return result;
}

int *get_data(char *line, int line_number, short *num_of_numbers) {
    char *ptr = NULL;
    char *cpy = NULL;
    int *numbers = NULL;
    char *keyword = ".data";

    cpy = strdupli(line);
    ptr = strstr(cpy, ".data");
    ptr += strlen(keyword);

    ptr = remove_spaces(ptr);

    /* The data is empty */
    if (ptr[0] == '\0') {
        generate_error(ERROR_DATA_EMPTY, line_number, line);
        cleanup("ss", cpy, ptr);
        return NULL;
    }

    /* There is a comma before the first number */
    if (ptr[0] == ',') {
        generate_error(ERROR_DATA_COMMA_AT_START, line_number, line);
        cleanup("ss", cpy, ptr);
        return NULL;
    }

    /* There is a comma after the last number */
    if (ptr[strlen(ptr) - 1] == ',') {
        generate_error(ERROR_DATA_COMMA_AT_END, line_number, line);
        cleanup("ss", cpy, ptr);
        return NULL;
    }

    /* There is multiple commas in a row */
    if (check_multiple_commas(cpy) == true) {
        generate_error(ERROR_DATA_MULTIPLE_COMMAS, line_number, line);
        cleanup("ss", cpy, ptr);
        return NULL;
    }

    /* Get that numbers */
    numbers = get_numbers(ptr, line_number, line, num_of_numbers);
    cleanup("ss", cpy, ptr);
    return numbers;

}

int *get_numbers(char *cpy, int line_number, char *line, short *num_of_numbers) {
    char *token = NULL;
    int capacity = 5;
    short count = 0;
    int *numbers = NULL;
    int *temp = NULL;

    numbers = (int *) malloc(capacity * sizeof(int));
    if (numbers == NULL) {
        generate_error(ERROR_MALLOC_FAILED, -1, "");
        return NULL;
    }

    /* TODO: check if the number is above X - check the חוברת*/
    /* Tokenizing by ',' */
    token = strtok(cpy, ",");
    while (token != NULL) {
        if (is_whole_number(token)) {
            /* There is not enough space. reallocate */
            if (count >= capacity) {
                capacity *= 2;
                temp = realloc(numbers, capacity * sizeof(int));
                if (temp == NULL) {
                    free(numbers);
                    generate_error(ERROR_MALLOC_FAILED, -1, "");
                    return NULL;
                } else {
                    numbers = temp;
                }
            }
            numbers[count] = atoi(token);
            count++;
        } else {
            cleanup("n",numbers);
            generate_error(ERROR_DATA_INVALID_INPUT, line_number, line);
            return NULL;
        }
        token = strtok(NULL, ",");
    }

    *num_of_numbers = count;
    return numbers;


}


