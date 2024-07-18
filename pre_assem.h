/* Pre assembler header file */
/* Date of creation - 3/7/24 */
#ifndef PRE_ASSEM
#define PRE_ASSEM

#include "data_structures.h"

/*
 * Function that starts the pre assembler process.
 * @param file_name - the file name of the file to start the pre assembler on.
 * @return True if the pre assembler succeeded, False otherwise.
 */
int start_pre_assem(char *file_name);

/*
 * Function that reads the file, looking for macro declarations and store them in the linked list.
 * @param fp - Pointer of the file that it reads.
 * @param head - Double pointer to the head of the list.
 * @return True if adding the macros succeeded, False otherwise.
 */
int add_macros_to_list(FILE *fp, Node **head);

/*
 * Function that reads the content of the macro to save them as a string.
 * @param fp - Pointer of the file that it reads.
 * @param pos - Pointer to the position inside the file.
 * @param line_number - Pointer to the line number inside the file.
 * @return The macro content.
 */
char *save_macro_content(FILE *fp, fpos_t *pos, int *line_number);


#endif
