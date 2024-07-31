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
bool line_is_comment(char *line);

/*
 * Function that constructs an entry/extern label node from a given line.
 * @param line - The line to check.
 * @param macro_head - Double pointer to the head of the macro list.
 * @param label_head - Double pointer to the head of the label list.
 * @param line_number - The number of the line given in the file.
 * @param num_of_entries - Pointer to the number of entry labels in the program.
 * @param num_of_externs - Pointer to the number of extern lables in the program.
 * @return True if it succeeded, false otherwise.
 */
bool construct_extern_entry(char *line, Node **macro_head, Node **label_head, int line_number, int *num_of_entries,
                            int *num_of_externs);

/*
 * Function that constructs instruction from a given line.
 * @param line - The line to check.
 * @param macro_head - Double pointer to the head of the macro list.
 * @param label_head - Double pointer to the head of the label list.
 * @param line_number - The number of the line given in the file.
 * @return Pointer to the instrParts created, NULL otherwise.
 */
instrParts *construct_instruction(char *line, Node **macro_head, Node **label_head, int line_number);

/*
 * Function that checks if there is a label declaration in the line.
 * @param line - the line to check.
 * @return The name of the label, NULL otherwise.
 */
char * line_is_label_decl(char * line);
/*
 * Function that validates the legality of the label name.
 * @param label_name - The label_name to check.
 * @param macro_head - Double pointer to the head of the macro list.
 * @return NO_ERROR if the name is valid, error_number otherwise.
 */
int validate_label_name(char *label_name, Node **macro_head);

#endif
