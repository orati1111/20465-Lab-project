/* Errors file */
/* Date of creation - 10/7/24 */

#include <stdio.h>
#include "errors.h"

/* The mapping of the errors */
errorMapping error_map[] = {{ERROR_ILLEGAL_MACRO_NAME,          ERR_MSG_ILLEGAL_MACRO_NAME},
                            {ERROR_EXTRA_TEXT_AFTER_MACRO_NAME, ERR_MSG_EXTRA_TEXT_AFTER_MACRO_NAME},
                            {ERROR_EXTRA_TEXT_AFTER_ENDMACR,    ERR_MSG_EXTRA_TEXT_AFTER_ENDMACR},
                            {ERROR_INVALID_MACRO_DECL,          ERR_MSG_INVALID_MACRO_DECL},
                            {ERROR_MACRO_ALREADY_EXISTS,        ERR_MSG_MACRO_ALREADY_EXISTS},
                            {ERROR_LONG_MACRO_NAME,             ERR_MSG_LONG_MACRO_NAME},
                            {ERROR_COULDNT_CREATE_MACRO_NODE,   ERR_MSG_COULDNT_CREATE_MACRO_NODE},
                            {ERROR_LINE_TOO_LONG,               ERR_MSG_LINE_TOO_LONG},
                            {ERROR_LABEL_NAME_TOO_LONG,ERR_MSG_LABEL_NAME_TOO_LONG},
                            {ERROR_ILLEGAL_LABEL_NAME,ERR_MSG_ILLEGAL_LABEL_NAME},
                            {ERROR_COULDNT_OPEN_FILE,           ERR_MSG_COULDNT_OPEN_FILE},
                            {ERROR_COULDNT_SET_POS_POINTER,     ERR_MSG_COULDNT_SET_POS_POINTER},
                            {ERROR_COULDNT_WRITE_AM_FILE,       ERR_MSG_COULDNT_WRITE_AM_FILE},
                            {ERROR_MALLOC_FAILED,               ERR_MSG_MALLOC_FAILED},
                            {ERROR_REALLOC_FAILED,              ERR_MSG_REALLOC_FAILED}
};

void generate_error(int error_number, int line_num, char *line) {
    /* Not a line error */
    if (line_num == -1)
        printf("ERROR - internal -  reason: %s\n", error_map[error_number].error_message);
    else if (error_number == ERROR_LINE_TOO_LONG)
        printf("ERROR - line:%d: reason: %s\n", line_num, error_map[error_number].error_message);
    else
        printf("ERROR - line:%d: %s      reason: %s\n", line_num, line, error_map[error_number].error_message);

}