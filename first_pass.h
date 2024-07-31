/* First pass header file */
/* Date of creation - 18/7/2024 */

#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include "utils.h"

/*
 * Function that starts the first pass for a given file.
 * @param file_name - The file name of the file to first pass on.th
 * @param macro_head - Double pointer to e head of the macro list.
 * @param label_head - Double pointer to e head of the label list.
 * @return True if the first pass succeeded , False otherwise.
 */
bool start_first_pass(char * file_name, Node ** macro_head , Node ** label_head);

#endif
