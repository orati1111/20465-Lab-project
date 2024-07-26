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
#define MAX_CHAR_IN_LINE 81
#define NUM_OF_OPS 16
#define NUM_OF_REGISTERS 9
#define MAX_MSG_LENGTH 256
#define MAX_MACRO_LENGTH 31
#define INITIAL_CONTENT_SIZE 64


/*
 * Function that adds a file extension to a given file name.
 * @param base_file_name - The base name of the file.
 * @param extension - The extension to add to the file name.
 * @return The full file name with extension.
 */
char *add_file_extension(char *base_file_name, char *extension);

/*
 * Function that checks if a given macro name is legal.
 * @param macr_name - The macro name to check.
 * @return True if the name is legal, False otherwise.
 */
bool is_macro_name_legal(char *macr_name);

/*
 * Function that replicates the strdup functionality(Which isn't C90).
 * @param original - The string to duplicate.
 * @return The duplicated string.
 */
char *strdupli(char *original);

/*
 * Function that removes leading or trailing spaces from a given string.
 * @param str - The string to remove spaces from.
 * @return The string without spaces
 */
char *remove_spaces(const char *str);

/*
 * Function that expands the macros, removes macro declarations and creates the am file.
 * @param fp_as - Pointer to the as file.
 * @param original_file_name - The original name of the file without extension.
 * @param head - Pointer to the head of the list.
 * @return True if it succeeded, false otherwise.
 */
bool macro_expansion(FILE *fp_as, char *original_file_name, Node *head);

/*
 * Function that handles the removal and replacement of the macros content and the macros.
 * @param fp_as - Pointer to the 'as' file.
 * @param tmp_file - The temp file name to store the text after removing the macro declaration.
 * @param original_file_name - The original file name without the extension.
 * @param head - Pointer to the head of the linked list.
 * @return True if the both the replacement and removal succeeded, False otherwise.l
 */
bool remove_and_replace_macros(FILE *fp_as, char *tmp_file, char *original_file_name, Node *head);

/*
 * Function that converts the operation code into binary and stores it inside the appropriate place
 * in the code_word.
 * @param word - Pointer to the code_word to set the bits in.
 * @param opcode - The operation code (char to be more efficient with memory).
 */
void set_opcode_binary(codeWord *word, char opcode);

/*
 * Function that checks if a given operation name is legal.
 * @param op_name - The operation name to check.
 * @return The op code that represents the operation name, -1 otherwise.
 */
int is_op_name(char *op_name);

#endif
