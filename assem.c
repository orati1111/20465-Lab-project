/* The main file for the project - The assembler. */
/* Date of creation - 3/7/2000 */

#include <stdio.h>
#include <stdlib.h>
#include "pre_assem.h"
#include "utils.h"


int main(int argc, char *argv[]) {
    char *as_file , *am_file;
    int index=1;
    /* Iterating through all the given files. */
    while(--argc > 0){
        printf("Pre Assembling file: %s\n", argv[index]);
        as_file = add_file_extension(argv[index++],AS_EXTENSION);
        printf("%s\n",as_file);

        /* In case there was an error while reading a file, continue. */
        if (!start_pre_assem(as_file)){
            continue;
        }
    }
    return 1;
}