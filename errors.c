/* Errors file */
/* Date of creation - 10/7/24 */

#include <stdio.h>
#include "errors.h"

error_mapping error_map[] = {{ERROR_ILLEGAL_MACRO_NAME,ERR_MSG_ILLEGAL_MACRO_NAME},
                             {ERROR_INVALID_MACRO_DECL,ERR_MSG_INVALID_MACRO_DECL},
                             {ERROR_COULDNT_OPEN_FILE,ERR_MSG_COULDNT_OPEN_FILE},
                             {ERROR_MALLOC_FAILED,ERR_MSG_MALLOC_FAILED},
                             {ERROR_EXTRA_TEXT_AFTER_DECL,ERR_MSG_EXTRA_TEXT_AFTER_DECL},
                            };

void generate_error(int error_number,int line_number){
    printf("ERROR detected at line %d, reason: %s\n",line_number,error_map[error_number].error_message);


}