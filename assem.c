/* The main file for the project - The assembler. */
/* Date of creation - 3/7/24 */

#include <stdio.h>
#include "data_structures.h"
#include "pre_assem.h"
#include "first_pass.h"
#include "second_pass.h"


/* TODO: add error and fix that it wont continue if pre assem has failed */
int main(int argc, char *argv[]) {
    char *raw_file_name = NULL;
    Node *macro_head = NULL;
    Node *label_head = NULL;
    Node *unknown_label_head = NULL;
    unsigned short IC = 0, DC = 0;
    codeWord memory[MAX_MEMORY_SIZE];
    bool no_error = true;

    int memory_ic_index = 0;
    int memory_dc_index = MAX_MEMORY_SIZE - 1;
    int index = 1;

    /* Iterating through all the given files. */
    while (index < argc) {
        macro_head = NULL;
        label_head = NULL;
        raw_file_name = argv[index++];

        printf("\nPre Assembling file: %s\n", raw_file_name);
        printf("In file: %s.as\n\n", raw_file_name);

        no_error = start_pre_assem(raw_file_name, &macro_head);

        /* Continue to the second pass only if */
        if (no_error == true) {
            printf("\nFirst Passing file: %s\n", raw_file_name);
            printf("In file: %s.am\n\n", raw_file_name);

            no_error = start_first_pass(raw_file_name, &macro_head, &label_head, (codeWord *) &memory,
                                        &unknown_label_head,
                                        &IC, &DC, &memory_ic_index, &memory_dc_index);

            printf("\nStart Second Passing file : %s\n", raw_file_name);
            printf("In file: %s.am\n\n", raw_file_name);

            start_second_pass(raw_file_name, &unknown_label_head, &label_head, (codeWord *) memory,
                              &IC, &DC, no_error, &memory_ic_index, &memory_dc_index);
        }


    }
    /* TODO: why this isnt getting freed? */
    free_list(&label_head, LABEL, ALL);
    free_list(&unknown_label_head, UNKNOWN_LABEL, ALL);
    return 1;
}