#ifndef GLOBALS_H
#define GLOBALS_H

#define ENTRY_TO_LOCAL 0
#define LOCAL_TO_ENTRY 1

/* Boolean */
typedef enum {
    false,
    true
} bool;

/* The types of the labels */
typedef enum {
    EXTERN,
    ENTRY,
    LOCAL
} labelType;

/* The type of the instruction */
typedef enum {
    DATA,
    STRING,
    NONE
} instrType;

/* The types of address_modes */
typedef enum {
    IMMEDIATE,
    DIRECT,
    INDIRECT_REG,
    DIRECT_REG,
    INIT,
    NOT_LEGAL

} addressMode;

/* Types of nodes in the linked list */
typedef enum {
    MACRO,
    LABEL,
    UNKNOWN_LABEL
} nodeType;

/* Free type */
typedef enum {
    CONTENT_ONLY,
    ALL
} freeType;

/* A,R,E field */
typedef enum {
    A = 4,
    R = 2,
    E = 1
} areField;

/* Where to encode the number inside the word */
/*
 * DST - bit 3-5
 * SRC - bit 6-8
 * FULL - bit 0-14
 * OTHER - bit 3-14
 */
typedef enum{
    DST,
    SRC,
    FULL,
    OTHER
} numberEncodeType;

#endif
