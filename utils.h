/* Utils header file */
/* Date of creation 3/7/24 */

#ifndef UTILS
#define UTILS

#define AS_EXTENSION ".as"
#define AM_EXTENSION ".am"
#define MAX_CHAR_IN_LINE 81
#define RESERVED_WORDS_NUMBER 25

typedef enum {
    false,
    true
} bool ;

char * add_file_extension(char *, char *);
void * generalized_malloc(long);
bool is_macro_name_legal(char *);



#endif
