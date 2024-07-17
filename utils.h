/* Utils header file */
/* Date of creation 3/7/24 */

#ifndef UTILS
#define UTILS

#define AS_EXTENSION ".as"
#define AM_EXTENSION ".am"
#define TMP_EXTENSION ".tmp"
#define MAX_CHAR_IN_LINE 81
#define RESERVED_WORDS_NUMBER 26
#define MAX_MSG_LENGTH 256

#include "data_structures.h"

typedef enum {
    false,
    true
} bool;

char *add_file_extension(char *, char *);

void *generalized_malloc(long);

bool is_macro_name_legal(char *);

char * remove_spaces(const char *);

char *copy_text(FILE *, fpos_t *, unsigned long long);

bool remove_all_macro_decl(FILE * , char *);

bool replace_all_macro(char *, char *, Node **);

bool remove_and_replace_macros(FILE * , char *, char *, Node **);

#endif
