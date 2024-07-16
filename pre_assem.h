/* Pre assembler header file */
/* Date of creation - 3/7/24 */
#ifndef PRE_ASSEM
#define PRE_ASSEM

#include "data_structures.h"

int start_pre_assem(char *);

int add_macros_to_list(FILE *, Node **);

char *save_macro_content(FILE *, fpos_t *, int *);


#endif
