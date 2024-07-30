/* Header file for the lexer */
/* Date of creation - 28/7/2024 */


#ifndef LEXER_H
#define LEXER_H
#include "globals.h"
#include "data_structures.h"






/*
 * Function that checks if a given line is a comment and should be ignored.
 * @param line - The line to check.
 * @return True if the line is a comment, false otherwise.
 */
bool line_is_comment(char * line);

/*
 * Function that handles the label declaration
 */
bool handle_label_decl(char * line, Node ** macro_head, int line_number);


bool tokenize(char * line, Node ** macro_head, int line_number);

#endif
