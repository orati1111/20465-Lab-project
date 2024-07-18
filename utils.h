/* Utils header file */
/* Date of creation 3/7/24 */

#ifndef UTILS
#define UTILS

/* Defines */
#define AS_EXTENSION ".as"
#define AM_EXTENSION ".am"
#define TMP_EXTENSION ".tmp"
#define MAX_CHAR_IN_LINE 81
#define RESERVED_WORDS_NUMBER 26
#define MAX_MSG_LENGTH 256

#include "data_structures.h"

/* Boolean */
typedef enum {
    false,
    true
} bool;

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
void *generalized_malloc(long size_of_object);

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
 * @return True if the both the replacement and removal succeeded, False otherwise.
 */
bool remove_and_replace_macros(FILE *fp_as, char *tmp_file, char *original_file_name, Node **head);

#endif
