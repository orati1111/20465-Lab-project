#ifndef GLOBALS_H
#define GLOBALS_H

/* Boolean */
typedef enum {
    false,
    true
} bool;

typedef enum {
    EXTERN,
    ENTRY,
    OTHER
} labelType;

typedef enum {
    MACRO,
    LABEL
} nodeType;

typedef enum {
    CONTENT_ONLY,
    ALL
} freeType;
#endif
