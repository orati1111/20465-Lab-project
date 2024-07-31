/* The main file for the project - The assembler. */
/* Date of creation - 3/7/24 */

#include <stdio.h>
#include "data_structures.h"
#include "pre_assem.h"
#include "first_pass.h"


int main(int argc, char *argv[]) {
    char *raw_file_name= NULL;
    Node *macro_head;
    Node *label_head;
    int index = 1;
    /* Iterating through all the given files. */
    while (index < argc) {
        macro_head = NULL;
        label_head = NULL;
        raw_file_name = argv[index++];
        printf("\nPre Assembling file: %s\n", raw_file_name);
        /* In case there was an error while reading a file, continue. */
        if (start_pre_assem(raw_file_name,&macro_head) == false) {
            continue;
        }
        printf("\nFirst Passing file: %s\n", raw_file_name);

        if (start_first_pass(raw_file_name,&macro_head,&label_head) == false) {
            continue;
        }

    }
    return 1;
}