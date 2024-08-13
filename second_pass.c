/* File that configure the second pass. */
/* Date of creation - 13/8/24 */


#include <stdio.h>
#include "second_pass.h"
#include "errors.h"
#include "encode.h"


void
start_second_pass(char *raw_file_name, Node **unknown_labels_head, Node **label_head, codeWord memory[MAX_MEMORY_SIZE],
                  unsigned short *IC,
                  unsigned short *DC, bool first_pass_error, int *memory_ic_index, int *memory_dc_index) {


    int second_pass_error = NO_ERROR;
    char *obj_file = NULL;
    char *ext_file = NULL;
    char *ent_file = NULL;

    FILE *obj_ptr = NULL;

    /* TODO: remember to free it */
    obj_file = add_file_extension(raw_file_name, OBJ_EXTENSION);
    ext_file = add_file_extension(raw_file_name, EXT_EXTENSION);
    ent_file = add_file_extension(raw_file_name, ENT_EXTENSION);

    /* Updating the addresses of the stored labels */
    update_label_address(*label_head, *IC, *DC);

    /* Iterating the unknown labels */
    second_pass_error = iterate_unknown_labels(unknown_labels_head, label_head, memory);



    /*
     * TODO: extern? skip , store the {LABEL IC} in .ext file
     * TODO: entry? store the {LABEL, memory} in ,ent file.
     * TODO: free the label_head, free the unknown label head
     */


    /* Everything is ok*/
    if (first_pass_error == true && second_pass_error == NO_ERROR) {
        obj_ptr = fopen(obj_file, "w");

        if (obj_ptr == NULL) {
            generate_error(ERROR_COULDNT_OPEN_FILE, -1, "");
            cleanup("sss", obj_file, ext_file, ent_file);
            return;
        }
        write_object_file(obj_ptr, memory, *memory_ic_index, *memory_dc_index, IC, DC);
        cleanup("s", obj_file);

    }
        /* Error has occurred */
    else {
        /* TODO: free and remove files if needed */
    }

}

void update_label_address(Node *label_head, unsigned short IC, unsigned short DC) {
    int offset;
    Node *temp = label_head;
    labelNode *label_node = NULL;

    offset = IC + 100;
    while (temp != NULL) {
        label_node = (labelNode *) temp->data;
        if (label_node->is_label_command == true)
            label_node->address += 100;
        else {
            label_node->address += offset;
        }
        temp = temp->next;
    }

}

int iterate_unknown_labels(Node **unknown_label_head, Node **label_head, codeWord memory[MAX_MEMORY_SIZE]) {

    Node *temp = NULL, *found = NULL;;
    unknownLabelNode *unknown_label_node = NULL;
    labelNode *found_data = NULL;
    codeWord code;
    labelType type;

    temp = *unknown_label_head;

    while (temp != NULL) {
        unknown_label_node = (unknownLabelNode *) temp->data;
        init_code_word(&code);
        if ((found = search_node(*label_head, unknown_label_node->label_name, LABEL)) != NULL) {
            found_data = (labelNode *) found->data;
            type = found_data->label_type;
            if (type == EXTERN) {
                /* TODO: write to ext file {LABEL_NAME, IC+100} */
                set_are_field_binary(&code, E);
                memory[unknown_label_node->memory_index] = code;
                temp = temp->next;
                continue;
            } else if (type == LOCAL || type == ENTRY) {
                /* Encoding the address */
                set_are_field_binary(&code, R);
                convert_to_binary(found_data->address, &code, OTHER);
                /* Storing the encoded word */
                memory[unknown_label_node->memory_index] = code;
                if (type == ENTRY) {
                    /* TODO: write to the ent file {LABEL_NAME, ADDRESS}*/
                }
            }
        } else {
            generate_error(ERROR_UNKNOWN_LABEL, unknown_label_node->line_number, unknown_label_node->line);
            return ERROR_UNKNOWN_LABEL;
        }
        temp = temp->next;
    }
    return NO_ERROR;
}

void write_object_file(FILE *obj_file, codeWord memory[MAX_MEMORY_SIZE], int memory_ic_index, int memory_dc_index,
                       unsigned short *IC,
                       unsigned short *DC) {

    int i, j;
    int offset = *IC + *DC + 100;

    j = MAX_MEMORY_SIZE - 1, i = 0;
    fprintf(obj_file, "   %d %d\n", *IC, *DC);

    while (i + 100 < offset) {
        if (i < memory_ic_index) {
            fprintf(obj_file, "%04d %05d\n", i + 100, convert_binary_to_octal(memory[i]));
        } else {
            fprintf(obj_file, "%04d %05d\n", i + 100, convert_binary_to_octal(memory[j]));
            j--;
        }
        i++;
    }

    cleanup("f", obj_file);

}