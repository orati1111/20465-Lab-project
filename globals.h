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
    LOCAL
} labelType;

typedef enum {
    DATA,
    STRING,
    NONE
} instrType;

typedef enum {
    IMMEDIATE,
    DIRECT,
    INDIRECT_REG,
    DIRECT_REG

} addressMode;

typedef enum {
    MACRO,
    LABEL
} nodeType;

typedef enum {
    CONTENT_ONLY,
    ALL
} freeType;
#endif
