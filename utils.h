/* Utils header file */
/* Date of creation 3/7/24 */

#ifndef UTILS_H
#define UTILS_H

#include "data_structures.h"

/* Defines */
/* File Extensions */
#define AS_EXTENSION ".as"
#define AM_EXTENSION ".am"
#define TMP_EXTENSION ".tmp"

/* Limiters */
#define MAX_CHAR_IN_LINE 82
#define BUFFER_SIZE 256
#define NUM_OF_OPS 16
#define NUM_OF_REGISTERS 9
#define MAX_MSG_LENGTH 256
#define MAX_MACRO_LENGTH 31
#define INITIAL_CONTENT_SIZE 64
#define DATA_NUMBER_UPPER_LIMIT 32767
#define DATA_NUMBER_LOWER_LIMIT -16384
#define MAX_MEMORY_SIZE 4096


/*
 * Function that adds a file extension to a given file name.
 * @param base_file_name - The base name of the file.
 * @param extension - The extension to add to the file name.
 * @return The full file name with extension.
 */
char *add_file_extension(char *base_file_name, char *extension);

/*
 * Function that checks if a given name is legal.
 * @param name - The name to check.
 * @return True if the name is legal, False otherwise.
 */
bool is_name_legal(char *name);

/*
 * Function that checks if a given name is an operand name
 * @param name - The name.
 * @return The index of the op_code, -1 otherwise.
 */
short is_name_op_name(char *name);


/*
 * Function that checks if a given name is a register
 * @param name - The name.
 * @return True if it is, false otherwise.
 */
bool is_name_register(char *name);

/*
 * Function that replicates the strdup functionality(Which isn't C90).
 * @param original - The string to duplicate.
 * @return The duplicated string.
 */
char *strdupli(char *original);

/*
 * Function that removes all spaces from a string.
 * @param str - The string to remove spaces from.
 * @return The string without spaces
 */
char *remove_spaces(const char *str);

/*
 * Function that removes leading spaces from a given string in place.
 * @param str - The string.
 */
void remove_leading_spaces(char *str);

/*
 * Function that removes trailing spaces from a given string in place.
 * @param str - The string.
 */
void remove_trailing_spaces(char *str);

/*
 * Function that expands the macros, removes macro declarations and creates the am file.
 * @param fp_as - Pointer to the as file.
 * @param original_file_name - The original name of the file without extension.
 * @param head - Pointer to the head of the list.
 * @return True if it succeeded, false otherwise.
 */
bool macro_expansion(FILE *fp_as, char *original_file_name, Node *head);

/*
 * Function that checks if there is extra text after a given pointer.
 * @param ptr - Pointer to the location to start checking from.
 * @param string - The string to check.
 * @return True if there is, False otherwise.
 */
bool check_extra_text(char *ptr, char *string);

/*
 * Function that converts the operation code into binary and stores it inside the appropriate place
 * in the code_word.
 * @param word - Pointer to the code_word to set the bits in.
 * @param opcode - The operation code (char to be more efficient with memory).
 */
void set_opcode_binary(codeWord *word, char opcode);

/*
 * Function that frees all the given arguments using stdargs
 * @param order - The order of the given arguments , i.e "fssf" means file,string,string,file.
 */
void cleanup(const char *order, ...);

/*
 * Function that checks if a given input has multiple consecutive commas,
 * @param input - The string to check.
 * @return True if there is multiple commas, false otherwise.
 */
bool check_multiple_commas(char *input);

/*
 * Function that in a given string, if there is only one parenthesis.
 * @param input - The string to check.
 * @return True if there is, false otherwise.
 */
bool check_invalid_parentheses(char *input);

/*
 * Function that gets a number as a string (including + or - in front of the number) and checks if it is a whole number.
 * Since .data accepts only whole numbers (both negative and positive).
 * @param number - The string that represents the number.
 * @return True if it is, false otherwise
 */
bool is_whole_number(char *number);

#endif
