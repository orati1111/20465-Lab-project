/* File that configure the second pass. */
/* Date of creation - 13/8/24 */


#include <stdio.h>
#include <stdlib.h>
#include "second_pass.h"
#include "errors.h"
#include "encode.h"


void
start_second_pass(char *raw_file_name, Node **unknown_labels_head, Node **label_head, codeWord memory[MAX_MEMORY_SIZE],
                  unsigned short *IC,
                  unsigned short *DC, bool first_pass_error, int *memory_ic_index) {


    int second_pass_error = NO_ERROR;
    char *obj_file = NULL;
    FILE *obj_ptr = NULL;


    obj_file = add_file_extension(raw_file_name, OBJ_EXTENSION);

    /* Updating the addresses of the stored labels */
    update_label_address(*label_head, *IC, *DC);

    /* Iterating the unknown labels */
    second_pass_error = update_memory_write_ext_file(unknown_labels_head, label_head, memory, raw_file_name);


    /* Everything is ok*/
    if (first_pass_error == true && second_pass_error == NO_ERROR) {
        obj_ptr = fopen(obj_file, "w");

        if (obj_ptr == NULL) {
            generate_error(ERROR_COULDNT_OPEN_FILE, -1, "");
            cleanup("s", obj_file);
            return;
        }
        write_object_file(obj_ptr, memory, *memory_ic_index, IC, DC);
        last_iteration(raw_file_name, label_head);
        cleanup("s", obj_file);

    } else
        cleanup("s", obj_file);

    free_list(label_head, LABEL, ALL);
    free_list(unknown_labels_head, UNKNOWN_LABEL, ALL);
}

void update_label_address(Node *label_head, unsigned short IC, unsigned short DC) {
    int offset;
    Node *temp = label_head;
    labelNode *label_node = NULL;

    offset = IC + OFFSET;
    while (temp != NULL) {
        label_node = (labelNode *) temp->data;
        if (label_node->is_label_command == true)
            label_node->address += OFFSET;
        else {
            label_node->address += offset;
        }
        temp = temp->next;
    }

}

int update_memory_write_ext_file(Node **unknown_label_head, Node **label_head, codeWord memory[4096],
                                 char *raw_file_name) {

    Node *temp = NULL;
    Node *found = NULL;
    unknownLabelNode *unknown_label_node = NULL;
    labelNode *found_data = NULL;
    char *ext_file = NULL;
    codeWord code;
    labelType type;


    ext_file = add_file_extension(raw_file_name, EXT_EXTENSION);
    temp = *unknown_label_head;

    while (temp != NULL) {
        unknown_label_node = (unknownLabelNode *) temp->data;
        init_code_word(&code);
        if ((found = search_node(*label_head, unknown_label_node->label_name, LABEL)) != NULL) {
            found_data = (labelNode *) found->data;
            type = found_data->label_type;
            if (type == EXTERN) {
                write_ext_ent_file(ext_file, found_data->label_name, unknown_label_node->IC + OFFSET, type);
                set_are_field_binary(&code, E);
                memory[unknown_label_node->memory_index] = code;
            } else if (type == LOCAL || type == ENTRY) {
                /* Encoding the address */
                set_are_field_binary(&code, R);
                convert_to_binary(found_data->address, &code, OTHER);
                /* Storing the encoded word */
                memory[unknown_label_node->memory_index] = code;
            }
        } else {
            generate_error(ERROR_UNKNOWN_LABEL, unknown_label_node->line_number, unknown_label_node->line);
            cleanup("s", ext_file);
            return ERROR_UNKNOWN_LABEL;
        }
        temp = temp->next;
    }
    cleanup("s", ext_file);
    return NO_ERROR;
}

void write_object_file(FILE *obj_file, codeWord memory[4096], int memory_ic_index,
                       unsigned short *IC,
                       unsigned short *DC) {

    int i, j;
    int offset = *IC + *DC + OFFSET;

    j = MAX_MEMORY_SIZE - 1, i = 0;
    fprintf(obj_file, "  %d %d\n", *IC, *DC);

    while (i + 100 < offset) {
        /* Until reaching the memory_ic_index - all the commands are printed */
        if (i < memory_ic_index) {
            fprintf(obj_file, "%04d %05d\n", i + OFFSET, convert_binary_to_octal(memory[i]));
            /* Printing the data/instructions */
        } else {
            fprintf(obj_file, "%04d %05d\n", i + OFFSET, convert_binary_to_octal(memory[j]));
            j--;
        }
        i++;
    }

    cleanup("f", obj_file);
}

void write_ext_ent_file(char *file_name, char *label_name, unsigned short address, labelType type) {
    FILE *ptr = NULL;

    ptr = fopen(file_name, "a");
    if (ptr == NULL) {
        generate_error(ERROR_COULDNT_OPEN_FILE, -1, "");
        exit(1);
    }
    /* Printing to the files with given margins and sizes for the labels and addresses to align*/
    if (type == EXTERN)
        fprintf(ptr, "%*s%-*s %0*d\n", MARGIN, "", MAX_MACRO_LABEL_LENGTH, label_name, ADDRESS_SIZE, address);
    else
        fprintf(ptr, "%*s%-*s %*d\n", MARGIN, "", MAX_MACRO_LABEL_LENGTH, label_name, ADDRESS_SIZE, address);
    cleanup("f", ptr);
}

/* Note: I seperated the ext and ent iterations to 2 different functions
 * because extern labels can only be found as command arguments
 * and entry can be both command arguments and local declared.
 * so by iterating the unknown labels - local declared entry labels will be missed. */
void last_iteration(char *raw_file_name, Node **label_head) {
    char *ent_file = NULL;
    Node *temp = NULL;
    labelNode *label = NULL;

    temp = *label_head;
    ent_file = add_file_extension(raw_file_name, ENT_EXTENSION);

    while (temp != NULL) {
        label = (labelNode *) temp->data;
        if (label->label_type == ENTRY) {
            write_ext_ent_file(ent_file, label->label_name, label->address, ENTRY);
        }
        temp = temp->next;
    }
    cleanup("s", ent_file);
}