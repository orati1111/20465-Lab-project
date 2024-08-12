/* Errors header file. */
/* Date of creation - 10/7/24 */

#ifndef ERRORS_H
#define ERRORS_H

#include "utils.h"

/* Defines */
#define ERR_MSG_ILLEGAL_MACRO_NAME "Illegal macro name."
#define ERR_MSG_INVALID_MACRO_DECL "Invalid macro declaration."
#define ERR_MSG_COULDNT_OPEN_FILE "Couldn't open the file given."
#define ERR_MSG_MALLOC_FAILED "Malloc memory failed."
#define ERR_MSG_EXTRA_TEXT_AFTER_MACRO_NAME "Extra text after macro name declaration."
#define ERR_MSG_COULDNT_SET_POS_POINTER "Couldn't set the position pointer in the given file."
#define ERR_MSG_EXTRA_TEXT_AFTER_ENDMACR "Extra text after the end of macro declaration."
#define ERR_MSG_MACRO_ALREADY_EXISTS "Attempt to declare a macro with name that already exists in the macro list."
#define ERR_MSG_COULDNT_WRITE_AM_FILE "Couldn't write to the am file."
#define ERR_MSG_LONG_MACRO_NAME "The macro name given is too long (exceeds 31) "
#define ERR_MSG_COULDNT_CREATE_NODE "Couldn't create a node in the linked list."
#define ERR_MSG_LINE_TOO_LONG "The line exceeds the 80 characters limit."
#define ERR_MSG_REALLOC_FAILED "Reallocating memory failed."
#define ERR_MSG_LABEL_NAME_TOO_LONG "The label name exceeds the 31 characters limit."
#define ERR_MSG_ILLEGAL_LABEL_NAME "Invalid label name."
#define ERR_MSG_UNKNOWN "Unknown error."
#define ERR_MSG_LABEL_AS_MACRO_NAME "Trying to declare a label with a already existing macro's name."
#define ERR_MSG_EXTRA_TEXT_ENTRY_EXTERN "Extra text after entry or extern declaration."
#define ERR_MSG_EXTERN_AS_LOCAL "Attempt to declare an extern label as local label"
#define ERR_MSG_LABEL_ALREADY_EXISTS "Attempt to declare a label with name that already exists in the label list"
#define ERR_MSG_LOCAL_AS_EXTERN "Attempt to declare local label as extern label"
#define ERR_MSG_INVALID_STRING_FORMAT "In .string instruction, the string should be enclosed with \"string\". "
#define ERR_MSG_DATA_EMPTY "There are no numbers after .data instruction - should be one or more."
#define ERR_MSG_INVALID_COMMA "Invalid input - invalid comma placement."
#define ERR_MSG_MULTIPLE_COMMAS "Invalid input - multiple commas in a row."
#define ERR_MSG_DATA_INVALID_INPUT "Invalid .data input - accepts only whole numbers in [-16384,32767] (-x for negative numbers, +x or x for positive) seperated by commas"
#define ERR_MSG_EXTERN_ENTRY "A label cannot be both entry and extern."
#define ERR_MSG_UNKNOWN_OP_NAME "Unknown opcode detected."
#define ERR_MSG_LABEL_EMPTY_DECL "Empty label declaration."
#define ERR_MSG_INVALID_AMOUNT_OF_ARGS "Invalid amount of arguments to the given opcode."
#define ERR_MSG_INVALID_ADDRESS_MODE "Invalid address mode found for the given opcode."
#define ERR_MSG_INVALID_INPUT "Invalid input."
#define ERR_MSG_IMID_MODE_INVALID_NUMBER "In address mode immediate, The number should be whole in [-2048, 2047]"
#define ERR_MSG_IND_REG_INVALID "In address mode indirect register, There should be a register following the *."
#define ERR_MSG_UNKNOWN_REGISTER "Unknown register name, r0-r7 are allowed."

/* The Error to number mapping */
enum {
    /* Macro Errors */
    ERROR_ILLEGAL_MACRO_NAME = 0,
    ERROR_EXTRA_TEXT_AFTER_MACRO_NAME,
    ERROR_EXTRA_TEXT_AFTER_ENDMACR,
    ERROR_INVALID_MACRO_DECL,
    ERROR_MACRO_ALREADY_EXISTS,
    ERROR_LONG_MACRO_NAME,
    ERROR_COULDNT_CREATE_NODE,

    /* Line Errors */
    ERROR_LINE_TOO_LONG,
    ERROR_EXTRA_TEXT_AFTER_ENTRY_EXTERN,

    /* Label Errors */
    ERROR_LABEL_NAME_TOO_LONG,
    ERROR_ILLEGAL_LABEL_NAME,
    ERROR_LABEL_AS_MACRO_NAME,
    ERROR_EXTERN_AS_LOCAL,
    ERROR_LOCAL_AS_EXTERN,
    ERROR_LABEL_ALREADY_EXISTS,
    ERROR_EXTERN_ENTRY,
    ERROR_LABEL_EMPTY_DECL,

    /* Op and arguments Errors */
    ERROR_UNKNOWN_OP_NAME,
    ERROR_INVALID_AMOUNT_OF_ARGUMENTS,
    ERROR_INVALID_ADDRESS_MODE,
    ERROR_INVALID_INPUT,
    ERROR_IMID_MODE_INVALID_INPUT,
    ERROR_IND_REG_INVALID,
    ERROR_UNKNOWN_REGISTER,

    /* .String Errors */
    ERROR_INVALID_STRING_FORMAT,

    /* .Data Errors */
    ERROR_DATA_EMPTY,
    /* General */
    ERROR_INVALID_COMMA,
    ERROR_MULTIPLE_COMMAS,
    ERROR_DATA_INVALID_INPUT,

    /* File Errors */
    ERROR_COULDNT_OPEN_FILE,
    ERROR_COULDNT_SET_POS_POINTER,
    ERROR_COULDNT_WRITE_AM_FILE,

    /* Memory Errors */
    ERROR_MALLOC_FAILED,
    ERROR_REALLOC_FAILED,
    ERROR_UNKNOWN,




    /* No error */
    NO_ERROR


};

/**
 * Struct to define the error mapping
 * error_number - The number of the error.
 * error_message - The error itself.
 */
typedef struct errorMapping {
    int error_number;
    char error_message[MAX_MSG_LENGTH];
} errorMapping;

/**
 * Function that generates the error.
 * @param error_number - The number of the error (defined at the top of the file).
 * @param line_number - The line number in which the error occurred (-1 if it wasn't line related.)
 * @param line - The content of the line.
 */
void generate_error(int error_number, int line_number, char * line);

#endif
