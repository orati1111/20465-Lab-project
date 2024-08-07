/* Errors file */
/* Date of creation - 10/7/24 */

#include <stdio.h>
#include "errors.h"

/* The mapping of the errors */
errorMapping error_map[] = {{ERROR_ILLEGAL_MACRO_NAME,            ERR_MSG_ILLEGAL_MACRO_NAME},
                            {ERROR_EXTRA_TEXT_AFTER_MACRO_NAME,   ERR_MSG_EXTRA_TEXT_AFTER_MACRO_NAME},
                            {ERROR_EXTRA_TEXT_AFTER_ENDMACR,      ERR_MSG_EXTRA_TEXT_AFTER_ENDMACR},
                            {ERROR_INVALID_MACRO_DECL,            ERR_MSG_INVALID_MACRO_DECL},
                            {ERROR_MACRO_ALREADY_EXISTS,          ERR_MSG_MACRO_ALREADY_EXISTS},
                            {ERROR_LONG_MACRO_NAME,               ERR_MSG_LONG_MACRO_NAME},
                            {ERROR_COULDNT_CREATE_NODE,           ERR_MSG_COULDNT_CREATE_NODE},
                            {ERROR_LINE_TOO_LONG,                 ERR_MSG_LINE_TOO_LONG},
                            {ERROR_EXTRA_TEXT_AFTER_ENTRY_EXTERN, ERR_MSG_EXTRA_TEXT_ENTRY_EXTERN},
                            {ERROR_LABEL_NAME_TOO_LONG,           ERR_MSG_LABEL_NAME_TOO_LONG},
                            {ERROR_ILLEGAL_LABEL_NAME,            ERR_MSG_ILLEGAL_LABEL_NAME},
                            {ERROR_LABEL_AS_MACRO_NAME,           ERR_MSG_LABEL_AS_MACRO_NAME},
                            {ERROR_EXTERN_AS_LOCAL,               ERR_MSG_EXTERN_AS_LOCAL},
                            {ERROR_LOCAL_AS_EXTERN,               ERR_MSG_LOCAL_AS_EXTERN},
                            {ERROR_LABEL_ALREADY_EXISTS,          ERR_MSG_LABEL_ALREADY_EXISTS},
                            {ERROR_EXTERN_ENTRY,                  ERR_MSG_EXTERN_ENTRY},
                            {ERROR_LABEL_EMPTY_DECL,              ERR_MSG_LABEL_EMPTY_DECL},
                            {ERROR_UNKNOWN_OP_NAME,               ERR_MSG_UNKNOWN_OP_NAME},
                            {ERROR_INVALID_AMOUNT_OF_ARGUMENTS,   ERR_MSG_INVALID_AMOUNT_OF_ARGS},
                            {ERROR_INVALID_ADDRESS_MODE,          ERR_MSG_INVALID_ADDRESS_MODE},
                            {ERROR_INVALID_INPUT,                 ERR_MSG_INVALID_INPUT},
                            {ERROR_IMID_MODE_INVALID_INPUT,       ERR_MSG_IMID_MODE_INVALID_NUMBER},
                            {ERROR_IND_REG_INVALID,               ERR_MSG_IND_REG_INVALID},
                            {ERROR_UNKNOWN_REGISTER,              ERR_MSG_UNKNOWN_REGISTER},
                            {ERROR_INVALID_STRING_FORMAT,         ERR_MSG_INVALID_STRING_FORMAT},
                            {ERROR_DATA_EMPTY,                    ERR_MSG_DATA_EMPTY},
                            {ERROR_COMMA_AT_START,                ERR_MSG_COMMA_AT_START},
                            {ERROR_COMMA_AT_END,                  ERR_MSG_COMMA_AT_END},
                            {ERROR_MULTIPLE_COMMAS,               ERR_MSG_MULTIPLE_COMMAS},
                            {ERROR_DATA_INVALID_INPUT,            ERR_MSG_DATA_INVALID_INPUT},
                            {ERROR_COULDNT_OPEN_FILE,             ERR_MSG_COULDNT_OPEN_FILE},
                            {ERROR_COULDNT_SET_POS_POINTER,       ERR_MSG_COULDNT_SET_POS_POINTER},
                            {ERROR_COULDNT_WRITE_AM_FILE,         ERR_MSG_COULDNT_WRITE_AM_FILE},
                            {ERROR_MALLOC_FAILED,                 ERR_MSG_MALLOC_FAILED},
                            {ERROR_REALLOC_FAILED,                ERR_MSG_REALLOC_FAILED},
                            {ERROR_UNKNOWN,                       ERR_MSG_UNKNOWN}
};

void generate_error(int error_number, int line_num, char *line) {
    /* Not a line error */
    if (line_num == -1 || error_number == ERROR_MALLOC_FAILED || error_number == ERROR_COULDNT_CREATE_NODE)
        printf("ERROR - internal -  reason: %s\n", error_map[error_number].error_message);
    else if (error_number == ERROR_LINE_TOO_LONG)
        printf("ERROR - line:%d: reason: %s\n", line_num, error_map[error_number].error_message);
    else
        printf("ERROR - line:%d: %s      reason: %s\n", line_num, line, error_map[error_number].error_message);

}