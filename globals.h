#ifndef GLOBALS_H
#define GLOBALS_H

#define ENTRY_TO_LOCAL 0
#define LOCAL_TO_ENTRY 1

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
    DIRECT_REG,
    INIT,
    NOT_LEGAL

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
