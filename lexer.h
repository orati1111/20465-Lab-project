/* Header file for the lexer */
/* Date of creation - 28/7/2024 */


#ifndef LEXER_H
#define LEXER_H

#include "globals.h"
#include "data_structures.h"
#include "utils.h"

/*
 * Structure to map each operand with its op_code , number of arguments and allowed address modes
 *  char * op_name - The name of the operand.
 *  short n_args - The number of allowed arguments to a given operand.
 *  short src_mode[4] - Array that represents the allowed address modes for the source. for example [1,1,0,0] means only address modes 0 and 1 are allowed.
 *  short dst_mode[4] - Array that represents the allowed address modes for the destination.
 */
typedef struct opMapping{
    char * op_name;
    short n_args;
    short src_mode[NUM_OF_ADDRESS_MODES];
    short dst_mode[NUM_OF_ADDRESS_MODES];
} opMapping;

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
 * @param memory_counter - The location in memory (DC or IC).
 * @param replace_flag - Pointer to a flag that represents if a memory/type swap should occur
 * when an entry label exists and a declaration is found, replace the memory address.
 * when a local label exists and .entry was found, replace the label type.
 * @return NO_ERROR if the name is valid, error_number otherwise.
 */
int validate_label_decl(char *label_name, Node **macro_head, Node **label_head, labelType type, unsigned short memory_counter, int * replace_flag);

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

/*
 * Function that gets a string , separates it to arguments and checks if they are valid.
 * Validating the number of arguments, address modes according to the given op_code.
 * @param arguments - The string that contains the arguments (Could be NULL)
 * @param command - Pointer to the commandParts struct to store the information in.
 * @return Error number that represents an error that occurred during the function, NO_ERROR otherwise.
 */
int handle_arguments(char * arguments, commandParts * command,Node **macro_head);

/*
 * Function that counts the amount of arguments in a given string.
 * If there is an error not related to the amount of arguments, set it to the error pointer.
 * @param arguments - The string of arguments.
 * @param error - A pointer to the error variable.
 * @return The amount of arguments.
 */
int count_arguments(char *arguments, int *error);

/*
 * Function that validates the arguments given based on the amount presented.
 * Validating the src and dst address codes, the content of the arguments, etc.
 * @param arguments - The string of arguments.
 * @param command - A pointer to the commandParts struct
 * @param number_of_arguments - The amount of arguments that should be.
 * @return NO_ERROR if it succeeded, error_number otherwise.
 */
int validate_store_arguments(char * arguments, commandParts * command, int number_of_arguments, Node **macro_head);


/*
 * Function that gets an argument and checks to what address mode does it belong.
 * @param argument - The argument to check.
 * @param error - Pointer to the error.
 * @param macro_head - Double pointer to the head of the macro list.
 * @return The number of address_mode , error otherwise.
 */
int get_address_mode(char * argument, int * error , Node **macro_head);

#endif
