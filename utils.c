/* Utils source file */
/* Date of creation - 3/7/24 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
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
        generate_error(ERROR_MALLOC_FAILED, -1);
        free(finalized_file_name);
        return NULL;
    }
    /* Copying the content to add the extension */
    strcpy(finalized_file_name, base_file_name);
    strcat(finalized_file_name, extension);
    finalized_file_name[finalized_length] = '\0';
    return finalized_file_name;

}

bool is_macro_name_legal(char *macr_name) {
    int index;
    /* Iterating the arrays and comparing the given name to the reserved names */
    /* Operations names: */
    for (index = 0; index < NUM_OF_OPS; index++) {
        if (strcmp(op_names[index], macr_name) == 0)
            return false;
    }
    /* Registers names */
    for (index = 0; index < NUM_OF_REGISTERS; index++) {
        if (strcmp(registers[index], macr_name) == 0)
            return false;
    }
    /* Name as the macro declaration */
    if (strcmp(macr_name, "macr") == 0)
        return false;
    return true;

}

char *strdupli(char *original) {
    char *cpy = NULL;
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

bool macro_expansion(FILE *fp_as, char *original_file_name, Node *head) {
    char buffer[MAX_CHAR_IN_LINE];
    char cpy[MAX_CHAR_IN_LINE];
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
        generate_error(ERROR_COULDNT_WRITE_AM_FILE, -1);
        remove(am_file);
        return false;
    }
    /* Reading the file */
    while (fgets(buffer, MAX_CHAR_IN_LINE, fp_as)) {
        strcpy(cpy, buffer);
        token = strtok(cpy, " \n");
        /* Looking for the start of macro declaration */
        if (strncmp(token, "macr", 4) == 0) {
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
                fprintf(fp_am, "%s", buffer);
            }
        }

    }
    fclose(fp_am);
    free(am_file);
    return true;

}

void set_opcode_binary(codeWord *word, char opcode) {
    word->bits[1] |= (opcode << 3);
}

int is_op_name(char *op_name) {
    int op_code = -1;
    int index;
    for (index = 0; index < NUM_OF_OPS; index++) {
        if (strcmp(op_name, op_names[index]) == 0) {
            op_code = index;
            break;
        }
    }
    return op_code;
}