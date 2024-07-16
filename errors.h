/* Errors header file. */
/* Date of creation - 10/7/24 */

#ifndef ERRORS_H
#define ERRORS_H

#include "utils.h"

#define ERR_MSG_ILLEGAL_MACRO_NAME "Illegal macro name."
#define ERR_MSG_INVALID_MACRO_DECL "Invalid macro declaration."
#define ERR_MSG_COULDNT_OPEN_FILE "Couldn't open the file given."
#define ERR_MSG_MALLOC_FAILED "Malloc memory failed."
#define ERR_MSG_EXTRA_TEXT_AFTER_MACRO_NAME "Extra text after macro name declaration."
#define ERR_MSG_COULDNT_SET_POS_POINTER "Couldn't set the position pointer in the given file."
#define ERR_MSG_EXTRA_TEXT_AFTER_ENDMACR "Extra text after the end of macro declaration."
#define ERR_MSG_MACRO_ALREADY_EXISTS "Attempt to declare a macro with name that already exists in the macro list."
#define ERR_MSG_COULDNT_WRITE_AM_FILE "Couldn't write to the am file."

enum {
    /* Macro Errors */
    ERROR_ILLEGAL_MACRO_NAME = 0,
    ERROR_EXTRA_TEXT_AFTER_MACRO_NAME,
    ERROR_EXTRA_TEXT_AFTER_ENDMACR,
    ERROR_INVALID_MACRO_DECL,
    ERROR_MACRO_ALREADY_EXISTS,

    /* File Errors */
    ERROR_COULDNT_OPEN_FILE,
    ERROR_COULDNT_SET_POS_POINTER,
    ERROR_COULDNT_WRITE_AM_FILE,

    /* Memory Errors */
    ERROR_MALLOC_FAILED,


};

typedef struct error_mapping {
    int error_number;
    char error_message[MAX_MSG_LENGTH];
} error_mapping;

void generate_error(int, int);

#endif
