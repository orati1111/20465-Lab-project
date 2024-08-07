/* File for the lexer */
/* Date of creation - 28/7/2024 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "lexer.h"
#include "utils.h"
#include "errors.h"

opMapping op_mappings[NUM_OF_OPS] = {{"mov",  2, {1, 1, 1, 1}, {0, 1, 1, 1}},
                                     {"cmp",  2, {1, 1, 1, 1}, {1, 1, 1, 1}},
                                     {"add",  2, {1, 1, 1, 1}, {0, 1, 1, 1}},
                                     {"sub",  2, {1, 1, 1, 1}, {0, 1, 1, 1}},
                                     {"lea",  2, {0, 1, 0, 0}, {0, 1, 1, 1}},
                                     {"clr",  1, {0, 0, 0, 0}, {0, 1, 1, 1}},
                                     {"not",  1, {0, 0, 0, 0}, {0, 1, 1, 1}},
                                     {"inc",  1, {0, 0, 0, 0}, {0, 1, 1, 1}},
                                     {"dec",  1, {0, 0, 0, 0}, {0, 1, 1, 1}},
                                     {"jmp",  1, {0, 0, 0, 0}, {0, 1, 1, 0}},
                                     {"bne",  1, {0, 0, 0, 0}, {0, 1, 1, 0}},
                                     {"red,", 1, {0, 0, 0, 0}, {0, 1, 1, 1}},
                                     {"prn",  1, {0, 0, 0, 0}, {1, 1, 1, 1}},
                                     {"jsr",  1, {0, 0, 0, 0}, {0, 1, 1, 0}},
                                     {"rts",  0, {0, 0, 0, 0}, {0, 0, 0, 0}},
                                     {"stop", 0, {0, 0, 0, 0}, {0, 0, 0, 0}}};


bool line_is_comment(char *line) {
    return line[0] == ';';
}

/* TODO: add ic dc*/
bool construct_extern_entry(char *line, Node **macro_head, Node **label_head, int line_number, int *num_of_entries,
                            int *num_of_externs) {
    char *cpy = NULL;
    char *ptr = NULL;
    char *keyword = NULL;
    char *label_name = NULL;
    char *extra = NULL;
    Node *temp = NULL;
    labelType type;

    cpy = strdupli(line);
    if (cpy == NULL) {
        generate_error(ERROR_MALLOC_FAILED, -1, "");
        return false;
    }
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

    /* Handling the label declaration */
    temp = handle_label_decl(line, label_name, macro_head, label_head, type, line_number, 0);
    if (temp == NULL) {
        cleanup("s", cpy);
        return false;
    }

    if (type == EXTERN)
        (*num_of_externs)++;
    else {
        (*num_of_entries)++;
    }

    cleanup("s", cpy);
    return true;


}

instrParts *
construct_instruction(char *line, Node **macro_head, Node **label_head, int line_number, unsigned short DC) {
    char *cpy = NULL;
    char *ptr = NULL;
    char *raw_string = NULL;
    char *string = NULL;
    char *label_name = NULL;
    int *numbers = NULL;
    instrParts *instruction = NULL;
    Node *temp = NULL;

    short num_of_numbers = 0;

    instruction = malloc(sizeof(instrParts));
    if (instruction == NULL) {
        generate_error(ERROR_MALLOC_FAILED, -1, "");
        exit(0);
    }
    init_struct_parts(instruction, NULL);

    label_name = line_is_label_decl(line);
    if (label_name != NULL) {
        temp = handle_label_decl(line, label_name, macro_head, label_head, LOCAL, line_number, DC);
        if (temp == NULL) {
            cleanup("si", label_name, instruction);
            return NULL;
        } else {
            instruction->label = temp;
        }

    }

    /* Handling data instruction */
    if (strstr(line, ".data") != NULL) {
        instruction->type = DATA;
        cpy = strdupli(line);
        numbers = get_data(cpy, line_number, &num_of_numbers);
        if (numbers == NULL) {
            cleanup("iss", instruction, cpy, label_name);
            return NULL;
        }
        instruction->data.numbers = numbers;
        instruction->length.amount_of_numbers = num_of_numbers;
        cleanup("ss", label_name, cpy);


        /* Handling string instruction */
    } else if ((cpy = strstr(line, ".string")) != NULL) {
        instruction->type = STRING;
        raw_string = strdupli(cpy);
        ptr = raw_string;
        raw_string += strlen(".string");
        if (strchr(raw_string, '"') == NULL || check_invalid_parentheses(raw_string) ||
            ((string = get_string(raw_string)) == NULL)) {
            generate_error(ERROR_INVALID_STRING_FORMAT, line_number, line);
            free_list(label_head, LABEL, ALL);
            cleanup("iss", instruction, label_name, ptr);
            return NULL;
        }
        instruction->data.string = string;
        instruction->length.string_length = strlen(string) + 1;
        cleanup("ss", label_name, ptr);
    }


    return instruction;


}


commandParts *construct_command(char *line, Node **macro_head, Node **label_head, int line_number, unsigned short IC) {
    char *cpy = NULL;
    char *op_name = NULL;
    char *ptr = NULL;
    char *remaining = NULL;
    char *label_name = NULL;
    commandParts *command = NULL;
    Node *temp = NULL;
    short op_code = -1;
    int error = NO_ERROR;

    cpy = strdupli(line);
    if (cpy == NULL) {
        generate_error(ERROR_MALLOC_FAILED, -1, "");
        return NULL;
    }

    command = malloc(sizeof(commandParts));
    if (command == NULL) {
        generate_error(ERROR_MALLOC_FAILED, -1, "");
        return NULL;
    }
    label_name = line_is_label_decl(cpy);
    if (label_name != NULL) {
        temp = handle_label_decl(line, label_name, macro_head, label_head, LOCAL, line_number, IC);
        if (temp == NULL) {
            cleanup("ssc", cpy, label_name, command);
            return NULL;
        }
    }
    init_struct_parts(NULL, command);
    command->label = temp;
    /* There is a label declaration in the line */
    if (temp != NULL) {
        ptr = strchr(cpy, ':');
        ptr++;
        ptr = strdupli(ptr);
        remove_leading_spaces(ptr);
        if (ptr == NULL || *ptr == '\0') {
            generate_error(ERROR_LABEL_EMPTY_DECL, line_number, line);
            cleanup("sssc", label_name, cpy, ptr, command);
            return NULL;
        }
        op_name = strdupli(strtok(ptr, " "));
        remaining = strdupli(strtok(NULL, "\n"));
        cleanup("s", ptr);
    } else {
        op_name = strdupli(strtok(cpy, " "));
        remaining = strdupli(strtok(NULL, "\n"));
    }
    if (op_name != NULL) {
        remove_leading_spaces(op_name);
        remove_trailing_spaces(op_name);
        /* Checking if the name is a known op name */
        if ((op_code = is_name_op_name(op_name)) == -1) {
            generate_error(ERROR_UNKNOWN_OP_NAME, line_number, line);
            cleanup("sscs", label_name, op_name, command, remaining);
            return NULL;
        }
    }
    command->op_code = op_code;
    /* Get the arguments */
    if ((error = handle_arguments(remaining, command)) != NO_ERROR) {
        generate_error(error, line_number, line);
    }


    /* TODO: Store all the information inside the command and return it*/



    cleanup("ssss", remaining, op_name, cpy, label_name);


    return command;
}


char *line_is_label_decl(char *line) {
    char cpy[MAX_CHAR_IN_LINE];
    char *token = NULL;
    strcpy(cpy, line);
    if (strchr(cpy, ':') == NULL)
        return NULL;
    token = strtok(cpy, ":");
    remove_leading_spaces(token);
    if (token != NULL)
        token = strdupli(token);
    return token;
}

int validate_label_decl(char *label_name, Node **macro_head, Node **label_head, labelType type) {
    Node *temp = NULL;
    labelNode *found = NULL;

    /* Checking the length of the label */
    if (strlen(label_name) > MAX_MACRO_LENGTH) {
        return ERROR_LABEL_NAME_TOO_LONG;
    }
    /* Check if the name is a macro name , opcode or register.*/
    if (search_node(*macro_head, label_name, MACRO) != NULL || is_name_legal(label_name) == false ||
        is_name_alphanumeric(label_name) == false) {
        return ERROR_ILLEGAL_LABEL_NAME;
    }
    if ((temp = search_node(*label_head, label_name, LABEL)) != NULL) {
        found = (labelNode *) temp->data;
        /* Already existing label */
        if (found->label_type == LOCAL && type == LOCAL)
            return ERROR_LABEL_ALREADY_EXISTS;
        /* An attempt to declare a label as extern but its declared locally */
        if (found->label_type == LOCAL && type == EXTERN)
            return ERROR_LOCAL_AS_EXTERN;
        /* An attempt to declare an extern label as local */
        if (found->label_type == EXTERN && type == LOCAL)
            return ERROR_EXTERN_AS_LOCAL;

        if ((found->label_type == EXTERN && type == ENTRY) || (found->label_type == ENTRY && type == EXTERN))
            return ERROR_EXTERN_ENTRY;
    }
    return NO_ERROR;

}

Node *
handle_label_decl(char *line, char *label_name, Node **macro_head, Node **label_head, labelType type, int line_number,
                  unsigned short memory_counter) {
    int error = NO_ERROR;
    labelNode *label_node = NULL;
    Node *temp = NULL;

    if ((error = validate_label_decl(label_name, macro_head, label_head, type)) != NO_ERROR) {
        generate_error(error, line_number, line);
        return NULL;
    }
    label_node = create_label_node(label_name, memory_counter, type);
    if (label_node == NULL) {
        generate_error(ERROR_COULDNT_CREATE_NODE, -1, "");
        return NULL;
    }
    temp = create_node(label_node, sizeof(labelNode), LABEL);
    cleanup("l", label_node);
    if (temp != NULL) {
        add_node(label_head, temp);
        return temp;
    } else
        return NULL;

}

char *get_string(const char *line) {
    const char *ptr = NULL;
    char *start_ptr = NULL;
    char *end_ptr = NULL;
    char *result = NULL;
    size_t size;


    /* Validating if the string isn't enclosed with "*/
    ptr = line;
    /* From the start */
    while (isspace(*ptr) && *ptr != '\0')
        ptr++;

    if (*ptr != '"')
        return NULL;

    ptr = line + strlen(line) - 1;

    /* From the end */
    while (isspace(*ptr) && ptr >= line)
        ptr--;

    if (*ptr != '"')
        return NULL;

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

    remove_leading_spaces(ptr);
    remove_trailing_spaces(ptr);

    /* The data is empty */
    if (ptr[0] == '\0') {
        generate_error(ERROR_DATA_EMPTY, line_number, line);
        cleanup("s", cpy);
        return NULL;
    }

    /* There is a comma before the first number */
    if (ptr[0] == ',') {
        generate_error(ERROR_COMMA_AT_START, line_number, line);
        cleanup("s", cpy);
        return NULL;
    }

    /* There is a comma after the last number */
    if (ptr[strlen(ptr) - 1] == ',') {
        generate_error(ERROR_COMMA_AT_END, line_number, line);
        cleanup("s", cpy);
        return NULL;
    }

    /* There is multiple commas in a row */
    if (check_multiple_commas(cpy) == true) {
        generate_error(ERROR_MULTIPLE_COMMAS, line_number, line);
        cleanup("s", cpy);
        return NULL;
    }

    /* Get that numbers */
    numbers = get_numbers(ptr, line_number, line, num_of_numbers);
    cleanup("s", cpy);
    return numbers;

}

int *get_numbers(char *cpy, int line_number, char *line, short *num_of_numbers) {
    char *token = NULL;
    int capacity = 5;
    int number;
    short count = 0;
    int *numbers = NULL;
    int *temp = NULL;

    numbers = (int *) malloc(capacity * sizeof(int));
    if (numbers == NULL) {
        generate_error(ERROR_MALLOC_FAILED, -1, "");
        return NULL;
    }

    /* Tokenizing by ',' */
    token = strtok(cpy, ",");
    while (token != NULL) {
        remove_leading_spaces(token);
        remove_trailing_spaces(token);
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
            number = atoi(token);
            if (number > BITS_15_MAX_NUMBER || number < BITS_15_MIN_NUMBER) {
                cleanup("n", numbers);
                generate_error(ERROR_DATA_INVALID_INPUT, line_number, line);
                return NULL;
            } else {
                numbers[count] = number;
                count++;
            }

        } else {
            cleanup("n", numbers);
            generate_error(ERROR_DATA_INVALID_INPUT, line_number, line);
            return NULL;
        }
        token = strtok(NULL, ",");
    }

    *num_of_numbers = count;
    return numbers;

}

int handle_arguments(char *arguments, commandParts *command) {
    int op_code = command->op_code;
    char *token = NULL;
    int number_of_arguments;
    int error = NO_ERROR;


    /* Count the amount of arguments. */
    number_of_arguments = count_arguments(arguments, &error);
    /* In case an error not related to the amount of arguments has occurred. */
    if (error != NO_ERROR)
        return error;
    if (number_of_arguments != op_mappings[op_code].n_args) {
        error = ERROR_INVALID_AMOUNT_OF_ARGUMENTS;
        return error;
    }
    /* Validate the arguments, content and store them if needed */
    if ((error = validate_store_arguments(arguments, command, number_of_arguments)) != NO_ERROR) {
        return error;
    }


    /* TODO: Look for address codes */
    /* TODO: check arguments - register? label? number? */
    /* TODO: label? if it doesnt exist , continue to look for it in the second pass */
    /* TODO: label name is macro name? ERROR */
    /* Cases */
    return error;
}

int count_arguments(char *arguments, int *error) {
    char *cpy = NULL;
    char *clean_input = NULL;
    char *token = NULL;
    int count = 0;

    /* In case there are no arguments */
    if (arguments == NULL)
        return count;

    cpy = strdupli(arguments);
    if (cpy == NULL)
        return ERROR_MALLOC_FAILED;
    clean_input = remove_spaces(cpy);
    if (clean_input == NULL) {
        cleanup("s", cpy);
        *error = ERROR_MALLOC_FAILED;
    }

    if (check_multiple_commas(clean_input) == true) {
        *error = ERROR_MULTIPLE_COMMAS;
        cleanup("ss", cpy, clean_input);
        return count;
    }
    token = strtok(clean_input, ",");
    while (token != NULL) {
        count++;
        token = strtok(NULL, ",");
    }
    cleanup("ss", cpy, clean_input);
    return count;
}

int validate_store_arguments(char *arguments, commandParts *command, int number_of_arguments) {
    char *token = NULL;
    char *cpy = NULL;
    int op_code = command->op_code;
    int src_address_mode;
    int dst_address_mode;
    int error = NO_ERROR;

    /* No arguments needed to be checked */
    if (number_of_arguments == 0)
        return NO_ERROR;

    cpy = strdupli(arguments);
    if (cpy == NULL)
        return ERROR_MALLOC_FAILED;

    remove_leading_spaces(cpy);
    remove_trailing_spaces(cpy);

    if (number_of_arguments == 1) {
        dst_address_mode = get_address_mode(cpy, &error);
        cleanup("s", cpy);
        if (error != NO_ERROR) {
            return error;
        }
        else if (dst_address_mode == NOT_LEGAL) {
            return ERROR_INVALID_INPUT;
        }
            /* Checking if the address mode that was found is valid according to the opcode mapping */
        else if (op_mappings[op_code].dst_mode[dst_address_mode] != 1) {
            return ERROR_INVALID_ADDRESS_MODE;
        }
            /* Storing the argument*/
        else {
            command->dst_mode = dst_address_mode;
            command->dst = arguments;
            printf("MODE: %d\n", dst_address_mode);
            printf("ARG: '%s'\n", arguments);
        }
    } else if (number_of_arguments == 2) {
        printf("2: %s\n", cpy);
        cleanup("s", cpy);
    }


    return error;
}

int get_address_mode(char *argument, int *error) {
    char *ptr = NULL;
    char *temp = NULL;

    /* Immediate */
    if ((ptr = strchr(argument, '#')) != NULL) {
        ptr = strdupli(ptr);
        /* Getting a pointer to the start inorder to free it.*/
        temp = ptr;
        if (ptr == NULL) {
            *error = ERROR_MALLOC_FAILED;
            return NOT_LEGAL;
        }
        /* Moving the pointer to the next character */
        ptr++;
        /* TODO: maybe check space here? if its legal */
        remove_trailing_spaces(ptr);
        if (is_whole_number(ptr) == false) {
            *error = ERROR_IMID_MODE_INVALID_INPUT;
        }

        cleanup("s", temp);
        if(*error != NO_ERROR)
            return NOT_LEGAL;
        else
            return IMMEDIATE;

    }

        /* Direct */
    else if ((ptr = strchr(argument, '*')) != NULL) {
        ptr = strdupli(ptr);
        if (ptr == NULL) {
            *error = ERROR_MALLOC_FAILED;
            return NOT_LEGAL;
        }
        temp = ptr;
        ptr++;
        remove_trailing_spaces(ptr);

        if (is_name_register(ptr) == false) {
            *error = ERROR_UNKNOWN_REGISTER;
        }
        if (strlen(ptr) != 2) {
            *error = ERROR_IND_REG_INVALID;
        }
        cleanup("s",temp);
        if(*error != NO_ERROR){
            return NOT_LEGAL;
        }
        else{
            return INDIRECT_REG;
        }

    }

    /* Indirect register */


    /* Direct reg */


    return NOT_LEGAL;
}


