/* The main file for the project - The assembler. */
/* Date of creation - 3/7/24 */

#include <stdio.h>
#include <stdlib.h>
#include "pre_assem.h"
#include "utils.h"


int main(int argc, char *argv[]) {
    char * raw_file_name;
    /* Iterating through all the given files. */
    while(--argc > 0){
        printf("Pre Assembling file: %s\n", argv[argc]);
        raw_file_name = argv[argc];
        /* In case there was an error while reading a file, continue. */
        if (start_pre_assem(raw_file_name) == false){
            continue;
        }
    }
    return 1;
}