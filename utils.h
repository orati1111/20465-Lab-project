/* Utils header file */
/* Date of creation 3/7/24 */

#ifndef UTILS
#define UTILS

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

/* Op - Code Mapping */
#define MOV_CODE 0
#define CMP_CODE 1
#define ADD_CODE 2
#define SUB_CODE 3
#define LEA_CODE 4
#define CLR_CODE 5
#define NOT_CODE 6
#define INC_CODE 7
#define DEC_CODE 8
#define JMP_CODE 9
#define BNE_CODE 10
#define RED_CODE 11
#define PRN_CODE 12
#define JSR_CODE 13
#define RTS_CODE 14
#define STP_CODE 15


#include "data_structures.h"

/* Boolean */
typedef enum {
    false,
    true
} bool;

typedef enum {
    EXTERN,
    ENTRY,
    OTHER
}labelType;

typedef enum{
    MACRO,
    LABEL
} nodeType;

/*
 * Function that adds a file extension to a given file name.
 * @param base_file_name - The base name of the file.
 * @param extension - The extension to add to the file name.
 * @return The full file name with extension.
 */
char *add_file_extension(char *base_file_name, char *extension);

/*
 * Function that generalize the malloc function to various objects.
 * @param size_of_object - The size of the object to allocate.
 */
void *generalized_malloc(size_t size_of_object);

/*
 * Function that checks if a given macro name is legal.
 * @param macr_name - The macro name to check.
 * @return True if the name is legal, False otherwise.
 */
bool is_macro_name_legal(char *macr_name);

/*
 * Function that removes leading or trailing spaces from a given string.
 * @param str - The string to remove spaces from.
 * @return The string without spaces
 */
char *remove_spaces(const char *str);

/*
 * Function that copies text and store it inside a string.
 * @param fp - Pointer of the file to read the content from.
 * @param pos - Pointer to the position inside the file to start copying content from.
 * @param length - The length of the content to copy.
 * @return The string that stores the copied content.
 */
char *copy_text(FILE *fp, fpos_t *pos, unsigned long long length);

/*
 * Function that removes the macro declarations from the as file and creates and new file without it.
 * The Function places new lines where there was macro declaration.
 * @param fp_as - Pointer to the as file.
 * @param tmp_file - The name of the tmp file to copy the content to.
 * @return True if the removal succeeded, False otherwise.
 */
bool remove_all_macro_decl(FILE *fp_as, char *tmp_file);

/*
 * Function that finds macro names to replace with the content of the macro by creating an 'am' file.
 * @param tmp_file - The temp file name after the removal of the macro declaration.
 * @param original_file_name - The original file name without the extension.
 * @param head - Double pointer to the head of the linked list.
 * @return True if the replacement succeeded, False otherwise.
 */
bool replace_all_macro(char *tmp_file, char *original_file_name, Node **head);

/*
 * Function that handles the removal and replacement of the macros content and the macros.
 * @param fp_as - Pointer to the 'as' file.
 * @param tmp_file - The temp file name to store the text after removing the macro declaration.
 * @param original_file_name - The original file name without the extension.
 * @param head - Double pointer to the head of the linked list.
 * @return True if the both the replacement and removal succeeded, False otherwise.l
 */
bool remove_and_replace_macros(FILE *fp_as, char *tmp_file, char *original_file_name, Node **head);

/*
 * Function that converts the operation code into binary and stores it inside the appropriate place
 * in the code_word.
 * @param word - Pointer to the code_word to set the bits in.
 * @param opcode - The operation code (char to be more efficient with memory).
 */
void set_opcode_binary(code_word *word, char opcode);

/*
 * Function that checks if a given operation name is legal.
 * @param op_name - The operation name to check.
 * @return The op code that represents the operation name, -1 otherwise.
 */
int is_op_name(char * op_name);

#endif
