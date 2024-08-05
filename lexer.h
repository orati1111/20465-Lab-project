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
 * Function that constructs an instruction from a given line.
 * @param line - The line to check.
 * @param macro_head - Double pointer to the head of the macro list.
 * @param label_head - Double pointer to the head of the label list.
 * @param line_number - The number of the line given in the file.
 * @para, DC - The data counter of the memory.
 * @return Pointer to the instrParts created, NULL otherwise.
 */
instrParts *construct_instruction(char *line, Node **macro_head, Node **label_head, int line_number, unsigned short DC);

/*
 * Function that constructs a command from a given line.
 * @param line - The line to check.
 * @param macro_head - Double pointer to the head of the macro list.
 * @param label_head - Double pointer to the head of the label list.
 * @param line_number - The number of the line in the file.
 * @param IC - the command counter of the memory.
 * @return Pointer to the commandParts created, NULL otherwise.
 */
commandParts * construct_command(char *line, Node **macro_head, Node **label_head, int line_number, unsigned short IC);

/*
 * Function that checks if there is a label declaration in the line.
 * @param line - the line to check.
 * @return The name of the label, NULL otherwise.
 */
char *line_is_label_decl(char *line);

/*
 * Function that validates the legality of the label name.
 * @param label_name - The label_name to check.
 * @param macro_head - Double pointer to the head of the macro list.
 * @param label_head - Double pointer to the head of the label list.
 * @param labelType - The type of the label.
 * @return NO_ERROR if the name is valid, error_number otherwise.
 */
int validate_label_decl(char *label_name, Node **macro_head, Node **label_head, labelType type);

/*
 * Function that handles the whole process of label declaration, validation and creation.
 * @param line - The line of the label declaration.
 * @param label_name - The name of the label.
 * @param macro_head - Double pointer to the head of the macro list.
 * @param macro_head - Double pointer to the head of the label list.
 * @param type - The type of the label.
 * @param line_number - The number of the line.
 * @param memory_counter - The location in memory (DC or IC)
 * @param The node of the label, NULL otherwise.
 */
Node * handle_label_decl(char * line, char *label_name, Node **macro_head, Node **label_head, labelType type , int line_number,unsigned short memory_counter);
/*
 * Function that gets a .string argument and extracts the string.
 * @param line - The line to extract the argument from.
 * @return The argument, NULL otherwise.
 */
char *get_string(const char *line);


/*
 * Function that gets the .data string, validates it and returns the number array
 * @param line - The line to extract the argument from.
 * @param line_number - The line of the number in the file.
 * @param num_of_numbers - Pointer to the number of numbers inside the array.
 * @return An array of the numbers, NULL otherwise.
 */
int *get_data(char *line, int line_number, short * num_of_numbers);

/*
 * Function that gets the .data numbers, stores then inside an array and returns it.
 * @param cpy - The copy of the line to alter.
 * @param line_number - The line of the number in the file.
 * @param line - The line itself.
 * @param num_of_numbers - A pointer to store the amount of numbers.
 */
int * get_numbers(char * cpy, int line_number, char * line, short * num_of_numbers);

#endif
