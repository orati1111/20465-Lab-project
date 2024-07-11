/* 20465 - Lab - Project */
/* Date of Creation - 3/7/24 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

void read_test(char *file_name) {
    /* File Pointers for as and am files. */
    char *am_file;
    char *as_file;
    FILE *fp_as = NULL;
    FILE *fp_am = NULL;
    int c;

    as_file = add_file_extension(file_name, AS_EXTENSION);
    fp_as = fopen(as_file, "r");
    if (fp_as == NULL) {
        /* TODO: generate error - couldn't find file */
        exit(1);
    }

    am_file = add_file_extension(file_name, AM_EXTENSION);
    fp_am = fopen(am_file, "w");


    while ((c = fgetc(fp_as)) != EOF) {
        fputc(c, fp_am);
    }
    fclose(fp_as);
    fclose(fp_am);

}


int start_pre_assem(char *file_name) {
    read_test(file_name);
    return 1;
}