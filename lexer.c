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
    temp = handle_label_decl(line, label_name, macro_head, label_head, type, line_number, 0,false);
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

    /* Handling data instruction */
    if (strstr(line, ".data") != NULL) {
        instruction->type = DATA;
        cpy = strdupli(line);
        numbers = get_data(cpy, line_number, &num_of_numbers);
        if (numbers == NULL) {
            cleanup("is", instruction, cpy);
            return NULL;
        }
        instruction->data.numbers = numbers;
        instruction->length.amount_of_numbers = num_of_numbers;
        cleanup("s", cpy);


        /* Handling string instruction */
    } else if ((cpy = strstr(line, ".string")) != NULL) {
        instruction->type = STRING;
        raw_string = strdupli(cpy);
        ptr = raw_string;
        raw_string += strlen(".string");
        if (strchr(raw_string, '"') == NULL || check_invalid_parentheses(raw_string) ||
            ((string = get_string(raw_string)) == NULL)) {
            generate_error(ERROR_INVALID_STRING_FORMAT, line_number, line);
            cleanup("is", instruction, ptr);
            return NULL;
        }
        instruction->data.string = string;
        instruction->length.string_length = strlen(string) + 1;
        cleanup("s", ptr);
    }

    label_name = line_is_label_decl(line);
    if (label_name != NULL) {
        temp = handle_label_decl(line, label_name, macro_head, label_head, LOCAL, line_number, DC, false);
        if (temp == NULL) {
            cleanup("si", label_name, instruction);
            return NULL;
        }
    }

    instruction->label = temp;
    cleanup("s", label_name);
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

    init_struct_parts(NULL, command);

    /* If there is a label declaration */
    if ((ptr = strchr(cpy, ':')) != NULL) {
        ptr++;
        ptr = strdupli(ptr);
        remove_leading_spaces(ptr);
        if (ptr == NULL || *ptr == '\0') {
            generate_error(ERROR_LABEL_EMPTY_DECL, line_number, line);
            cleanup("ssc", cpy, ptr, command);
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

        /* Validating commas */
        error = validate_commas_format(op_name);
        if (error == NO_ERROR)
            error = validate_commas_format(remaining);
        if (error != NO_ERROR) {
            generate_error(error, line_number, line);
            cleanup("sscs", cpy, op_name, command, remaining);
            return NULL;
        }

        /* Checking if the name is a known op name */
        if ((op_code = is_name_op_name(op_name)) == -1) {
            generate_error(ERROR_UNKNOWN_OP_NAME, line_number, line);
            cleanup("scss", op_name, command, remaining, cpy);
            return NULL;
        }
    }


    command->op_code = op_code;
    /* Get the arguments */
    if ((error = handle_arguments(remaining, command, macro_head)) != NO_ERROR) {
        generate_error(error, line_number, line);
        cleanup("sscs", cpy, op_name, command, remaining);
        return NULL;
    }


    label_name = line_is_label_decl(cpy);
    if (label_name != NULL) {
        temp = handle_label_decl(line, label_name, macro_head, label_head, LOCAL, line_number, IC, true);
        if (temp == NULL) {
            cleanup("sscss", cpy, label_name, command, remaining, op_name);
            return NULL;
        }
    }

    command->label = temp;
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

int validate_label_decl(char *label_name, Node **macro_head, Node **label_head, labelType type,
                        unsigned short memory_counter, int *replace_flag, Node **ptr, bool is_label_command) {
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
        /* An attempt to declare extern as entry or vise versa */
        if ((found->label_type == EXTERN && type == ENTRY) || (found->label_type == ENTRY && type == EXTERN))
            return ERROR_EXTERN_ENTRY;
        /* Updating the memory location of an entry label after it declaration */
        if (found->label_type == ENTRY && type == LOCAL) {
            found->address = memory_counter;
            found->is_label_command = is_label_command;
            *replace_flag = ENTRY_TO_LOCAL;
            *ptr = temp;
        }
        if (found->label_type == LOCAL && type == ENTRY) {
            found->label_type = ENTRY;
            *replace_flag = LOCAL_TO_ENTRY;
            *ptr = temp;
        }

    }
    return NO_ERROR;

}

Node *
handle_label_decl(char *line, char *label_name, Node **macro_head, Node **label_head, labelType type, int line_number,
                  unsigned short memory_counter, bool is_label_command) {
    int error = NO_ERROR;
    int replace_flag = -1;
    labelNode *label_node = NULL;
    Node *temp = NULL;

    if ((error = validate_label_decl(label_name, macro_head, label_head, type, memory_counter, &replace_flag, &temp,is_label_command)) !=
        NO_ERROR) {
        generate_error(error, line_number, line);
        return NULL;
    }
    if (replace_flag != ENTRY_TO_LOCAL && replace_flag != LOCAL_TO_ENTRY) {
        label_node = create_label_node(label_name, memory_counter, type, is_label_command);
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
    return temp;

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
    int error = NO_ERROR;

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
    /* Validating the commas */
    if ((error = validate_commas_format(cpy)) != NO_ERROR) {
        generate_error(error, line_number, line);
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
                cleanup("a", numbers);
                generate_error(ERROR_DATA_INVALID_INPUT, line_number, line);
                return NULL;
            } else {
                numbers[count] = number;
                count++;
            }

        } else {
            cleanup("a", numbers);
            generate_error(ERROR_DATA_INVALID_INPUT, line_number, line);
            return NULL;
        }
        token = strtok(NULL, ",");
    }

    *num_of_numbers = count;
    return numbers;

}

int handle_arguments(char *arguments, commandParts *command, Node **macro_head) {
    int op_code = command->op_code;
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
    if ((error = validate_store_arguments(arguments, command, number_of_arguments, macro_head)) != NO_ERROR) {
        return error;
    }

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

int validate_store_arguments(char *arguments, commandParts *command, int number_of_arguments, Node **macro_head) {
    char *arg1 = NULL;
    char *arg2 = NULL;
    char *token = NULL;
    char *cpy = NULL;
    int op_code = command->op_code;
    int src_address_mode;
    int dst_address_mode;
    int error_src = NO_ERROR;
    int error_dst = NO_ERROR;

    /* No arguments needed to be checked */
    if (number_of_arguments == 0)
        return NO_ERROR;

    cpy = strdupli(arguments);
    if (cpy == NULL)
        return ERROR_MALLOC_FAILED;

    remove_leading_spaces(cpy);
    remove_trailing_spaces(cpy);

    if (number_of_arguments == 1) {
        dst_address_mode = get_address_mode(cpy, &error_dst, macro_head);
        if (error_dst != NO_ERROR) {
            return error_dst;

        } else if (dst_address_mode == NOT_LEGAL) {
            return ERROR_INVALID_INPUT;
        }
            /* Checking if the address mode that was found is valid according to the opcode mapping */
        else if (op_mappings[op_code].dst_mode[dst_address_mode] != 1) {
            return ERROR_INVALID_ADDRESS_MODE;
        }
            /* Storing the argument*/
        else {
            command->dst_mode = dst_address_mode;
            command->dst = cpy;
        }

    } else if (number_of_arguments == 2) {
        token = strdupli(cpy);
        cleanup("s", cpy);
        if (token == NULL)
            return ERROR_MALLOC_FAILED;

        arg1 = strtok(token, ",");
        arg2 = strtok(NULL, ",");
        src_address_mode = get_address_mode(arg1, &error_src, macro_head);
        dst_address_mode = get_address_mode(arg2, &error_dst, macro_head);

        /* Error found */
        if (error_src != NO_ERROR || error_dst != NO_ERROR) {
            cleanup("s", token);
            if (error_src != NO_ERROR)
                return error_src;
            return error_dst;
        }

        /* Address code not legal */
        if (src_address_mode == NOT_LEGAL || dst_address_mode == NOT_LEGAL) {
            cleanup("s", token);
            return ERROR_INVALID_INPUT;
        }

        /* Address code doesn't match the allowed address codes for the given opcode */
        if ((op_mappings[op_code].src_mode[src_address_mode] != 1) ||
            (op_mappings[op_code].dst_mode[dst_address_mode] != 1)) {
            cleanup("s", token);
            return ERROR_INVALID_ADDRESS_MODE;
        } else {
            /* Removing spaces */
            remove_leading_spaces(arg1);
            remove_trailing_spaces(arg1);
            remove_leading_spaces(arg2);
            remove_trailing_spaces(arg2);

            command->src_mode = src_address_mode;
            command->src = strdupli(arg1);
            command->dst_mode = dst_address_mode;
            command->dst = strdupli(arg2);

            /* Checking if both address modes are registers in order to encode them with 1 word later. */
            if ((src_address_mode == INDIRECT_REG || src_address_mode == DIRECT_REG) &&
                (dst_address_mode == INDIRECT_REG || dst_address_mode == DIRECT_REG))
                command->both_registers = true;

            cleanup("s", token);
        }
    }


    return NO_ERROR;
}

int get_address_mode(char *argument, int *error, Node **macro_head) {
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
        remove_trailing_spaces(ptr);
        if (is_whole_number(ptr) == false) {
            *error = ERROR_IMID_MODE_INVALID_INPUT;
        }

        cleanup("s", temp);
        if (*error != NO_ERROR)
            return NOT_LEGAL;
        else
            return IMMEDIATE;

    }

        /* Indirect register */
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
        cleanup("s", temp);
        if (*error != NO_ERROR) {
            return NOT_LEGAL;
        } else {
            return INDIRECT_REG;
        }

    }

        /* Direct reg */
    else if (strchr(argument, 'r') != NULL) {
        ptr = strdupli(argument);
        remove_leading_spaces(ptr);
        remove_trailing_spaces(ptr);
        if (is_name_register(ptr) == false)
            *error = ERROR_UNKNOWN_REGISTER;
        cleanup("s", ptr);
        if (*error != NO_ERROR)
            return NOT_LEGAL;
        else
            return DIRECT_REG;
    }
        /* Direct */
    else {
        ptr = strdupli(argument);
        remove_leading_spaces(ptr);
        remove_trailing_spaces(ptr);
        if (is_name_legal(ptr) == false || is_name_alphanumeric(ptr) == false ||
            search_node(*macro_head, ptr, MACRO) != NULL) {
            *error = ERROR_ILLEGAL_LABEL_NAME;
        }
        cleanup("s", ptr);
        if (*error != NO_ERROR)
            return NOT_LEGAL;
        else
            return DIRECT;
    }


}


