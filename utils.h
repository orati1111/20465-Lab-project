/* Utils header file */
/* Date of creation 3/7/24 */

#ifndef UTILS_H
#define UTILS_H

#include "data_structures.h"

/* Defines */
/* File Extensions */
#define AS_EXTENSION ".as"
#define AM_EXTENSION ".am"
#define OBJ_EXTENSION ".ob"
#define EXT_EXTENSION ".ext"
#define ENT_EXTENSION ".ent"

/* Limiters */
#define MAX_CHAR_IN_LINE 82
#define SIZE_OF_WORD 15
#define BUFFER_SIZE 256
#define NUM_OF_OPS 16
#define NUM_OF_REGISTERS 9
#define MAX_MSG_LENGTH 256
#define MAX_MACRO_LENGTH 31
#define INITIAL_CONTENT_SIZE 64
#define BITS_15_MAX_NUMBER 16383
#define BITS_15_MIN_NUMBER -16384
#define BITS_12_MAX_NUMBER 2047
#define BITS_12_MIN_NUMBER -2048
#define MAX_MEMORY_SIZE 4096
#define OFFSET 100
#define NUM_OF_ADDRESS_MODES 4


/**
 * Function that adds a file extension to a given file name.
 * @param base_file_name - The base name of the file.
 * @param extension - The extension to add to the file name.
 * @return The full file name with extension.
 */
char *add_file_extension(char *base_file_name, char *extension);

/**
 * Function that checks if a given name is legal.
 * @param name - The name to check.
 * @return True if the name is legal, False otherwise.
 */
bool is_name_legal(char *name);

/**
 * Function that checks if a given name is an operand name
 * @param name - The name.
 * @return The index of the op_code, -1 otherwise.
 */
short is_name_op_name(char *name);


/**
 * Function that checks if a given name is a register
 * @param name - The name.
 * @return True if it is, false otherwise.
 */
bool is_name_register(char *name);

/**
 * Function that checks the characters of a given string that they are only letters or numbers.
 * @param name - The name.
 * @return True if it contains only numbers and letters, false otherwise.
 */
bool is_name_alphanumeric(char *name);

/**
 * Function that replicates the strdup functionality(Which isn't C90).
 * @param original - The string to duplicate.
 * @return The duplicated string.
 */
char *strdupli(char *original);

/**
 * Function that removes all spaces from a string.
 * @param str - The string to remove spaces from.
 * @return The string without spaces
 */
char *remove_spaces(const char *str);

/**
 * Function that removes leading spaces from a given string in place.
 * @param str - The string.
 */
void remove_leading_spaces(char *str);

/**
 * Function that removes trailing spaces from a given string in place.
 * @param str - The string.
 */
void remove_trailing_spaces(char *str);

/**
 * Function that expands the macros, removes macro declarations and creates the am file.
 * @param fp_as - Pointer to the as file.
 * @param original_file_name - The original name of the file without extension.
 * @param head - Pointer to the head of the list.
 * @return True if it succeeded, false otherwise.
 */
bool macro_expansion(FILE *fp_as, char *original_file_name, Node *head);

/**
 * Function that checks if there is extra text after a given pointer.
 * @param ptr - Pointer to the location to start checking from.
 * @param string - The string to check.
 * @return True if there is, False otherwise.
 */
bool check_extra_text(char *ptr, char *string);


/**
 * Function that frees all the given arguments using stdargs
 * @param order - The order of the given arguments , i.e "fssf" means file,string,string,file.
 * options: s - char * , f - File , l - labelNode , i - instParts, c - commandParts, a - int * , m - macroNode, u - unknownLabelNode.
 */
void cleanup(const char *order, ...);

/**
 * Function that checks if a given input has multiple consecutive commas,
 * @param input - The string to check.
 * @return True if there is multiple commas, false otherwise.
 */
int check_multiple_commas(char *input);

/**
 * Function that in a given string, if there is only one parenthesis.
 * @param input - The string to check.
 * @return True if there is, false otherwise.
 */
bool check_invalid_parentheses(char *input);

/**
 * Function that gets a number as a string (including + or - in front of the number) and checks if it is a whole number.
 * Since .data accepts only whole numbers (both negative and positive).
 * @param number - The string that represents the number.
 * @return True if it is, false otherwise
 */
bool is_whole_number(char *number);

/**
 * Function that converts int input to binary and store them.
 * @param input - The number to convert to binary.
 * @param code - Pointer to code ward to store the binary in.
 * @param type - Represents where to encode the number bitwise.
 * DST - bit 3-5
 * SRC - bit 6-8
 * FULL - bit 0-14
 * OTHER - bit 3-14
 */
void convert_to_binary(int input, codeWord *code, numberEncodeType type);

/**
 * Function that converts binary to base 8.
 * @param code - The binary to convert
 * @return The number in base 8.
 */
int convert_binary_to_octal(codeWord code);

/**
 * Function that checks if there is an invalid comma at the start or the end.
 * @param line - The string to check
 * @return ERROR_INVALID_COMMA , NO_ERROR otherwise.
 */
int check_commas_at_start_end(char *line);

/**
 * Function that combines the check_commas_at_start_end and check_multiple_commas to one.
 * @param line - The string to check.
 * @return error_number according to the error found, NO_ERROR otherwise.
 */
int validate_commas_format(char *line);

/**
 * Function that gets 2 codeWords and returns a code that is BITWISE OR of the 2 codes.
 * @param code1 - First codeWord.
 * @param code2 - Second codeWord.
 * @return The bitwise OR codeWord.
 */
codeWord bitwise_or_codes(codeWord code1, codeWord code2);


#endif
