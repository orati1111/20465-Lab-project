/* Errors header file. */
/* Date of creation - 10/7/24 */

#ifndef ERRORS_H
#define ERRORS_H

#include "utils.h"

/* Defines */
#define ERR_MSG_ILLEGAL_MACRO_NAME "Illegal macro name."
#define ERR_MSG_INVALID_MACRO_DECL "Invalid macro declaration."
#define ERR_MSG_COULDNT_OPEN_FILE "Couldn't open the file given."
#define ERR_MSG_MALLOC_FAILED "Malloc memory failed."
#define ERR_MSG_EXTRA_TEXT_AFTER_MACRO_NAME "Extra text after macro name declaration."
#define ERR_MSG_COULDNT_SET_POS_POINTER "Couldn't set the position pointer in the given file."
#define ERR_MSG_EXTRA_TEXT_AFTER_ENDMACR "Extra text after the end of macro declaration."
#define ERR_MSG_MACRO_ALREADY_EXISTS "Attempt to declare a macro with name that already exists in the macro list."
#define ERR_MSG_COULDNT_WRITE_AM_FILE "Couldn't write to the am file."
#define ERR_MSG_LONG_MACRO_NAME "The macro name given is too long (exceeds 31) "
#define ERR_MSG_COULDNT_CREATE_MACRO_NODE "Couldn't create a node in the linked list for the macro."
#define ERR_MSG_LINE_TOO_LONG "The line exceeds the 80 characters limit."
#define ERR_MSG_REALLOC_FAILED "Reallocating memory failed."
#define ERR_MSG_LABEL_NAME_TOO_LONG "The label name exceeds the 31 characters limit."
#define ERR_MSG_ILLEGAL_LABEL_NAME "Invalid label name."

/* The Error to number mapping */
enum {
    /* Macro Errors */
    ERROR_ILLEGAL_MACRO_NAME = 0,
    ERROR_EXTRA_TEXT_AFTER_MACRO_NAME,
    ERROR_EXTRA_TEXT_AFTER_ENDMACR,
    ERROR_INVALID_MACRO_DECL,
    ERROR_MACRO_ALREADY_EXISTS,
    ERROR_LONG_MACRO_NAME,
    ERROR_COULDNT_CREATE_MACRO_NODE,

    /* Line Errors */
    ERROR_LINE_TOO_LONG,

    /* Label Errors */
    ERROR_LABEL_NAME_TOO_LONG,
    ERROR_ILLEGAL_LABEL_NAME,

    /* File Errors */
    ERROR_COULDNT_OPEN_FILE,
    ERROR_COULDNT_SET_POS_POINTER,
    ERROR_COULDNT_WRITE_AM_FILE,

    /* Memory Errors */
    ERROR_MALLOC_FAILED,
    ERROR_REALLOC_FAILED


};

/* Struct to define the error mapping
 * error_number - The number of the error.
 * error_message - The error itself.
 */
typedef struct errorMapping {
    int error_number;
    char error_message[MAX_MSG_LENGTH];
} errorMapping;

/*
 * Function that generates the error.
 * @param error_number - The number of the error (defined at the top of the file).
 * @param line_number - The line number in which the error occurred (-1 if it wasn't line related.)
 * @param line - The content of the line.
 */
void generate_error(int error_number, int line_number, char * line);

#endif
