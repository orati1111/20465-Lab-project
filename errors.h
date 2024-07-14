/* Errors header file. */
/* Date of creation - 10/7/24 */

#ifndef ERRORS_H
#define ERRORS_H

#include "utils.h"

#define ERR_MSG_ILLEGAL_MACRO_NAME "Illegal macro name."
#define ERR_MSG_INVALID_MACRO_DECL "Invalid macro declaration."
#define ERR_MSG_COULDNT_OPEN_FILE "Couldn't open the file given."
#define ERR_MSG_MALLOC_FAILED "Malloc memory failed."
#define ERR_MSG_EXTRA_TEXT_AFTER_DECL "Extra text after macro declaration."

enum {
    /* Macro Errors */
    ERROR_ILLEGAL_MACRO_NAME = 0,
    ERROR_EXTRA_TEXT_AFTER_DECL,
    ERROR_INVALID_MACRO_DECL,

    /* File Errors */
    ERROR_COULDNT_OPEN_FILE,

    /* Memory Errors */
    ERROR_MALLOC_FAILED,


};

typedef struct error_mapping {
    int error_number;
    char error_message[MAX_MSG_LENGTH];
} error_mapping;

void generate_error(int, int);

#endif
