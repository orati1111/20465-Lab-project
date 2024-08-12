/* encode source file */
/* Date of creation - 11/8/24 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "encode.h"


void
encode_instruction(instrParts *instruction, codeWord memory[MAX_MEMORY_SIZE], int *memory_index, unsigned short *DC) {
    codeWord code;
    instrType type = instruction->type;
    size_t length;
    void *input;
    int i;

    init_code_word(&code);

    if (type == DATA) {
        length = instruction->length.amount_of_numbers;
        input = instruction->data.numbers;
    } else {
        length = instruction->length.string_length;
        input = instruction->data.string;
    }

    for (i = 0; i < length; i++) {
        init_code_word(&code);
        if (type == DATA) {
            convert_to_binary(((int *) input)[i], &code, FULL);
        } else {
            convert_to_binary(((char *) input)[i], &code, FULL);
        }

        memory[*memory_index] = code;
        (*memory_index)--;
        (*DC)++;

    }

}

/* TODO: add the Unknown labels here and create a function that increments the size. */
void encode_command(commandParts *command, codeWord memory[MAX_MEMORY_SIZE], int *memory_index, unsigned short *IC,
                    Node **label_head, Node **unknown_label_head, char *line, int line_number) {
    char *src = command->src;
    char *dst = command->dst;
    short op_code = command->op_code;
    addressMode src_mode = command->src_mode;
    addressMode dst_mode = command->dst_mode;
    bool both_registers = command->both_registers;

    /* First word */
    encode_first_word(src, dst, src_mode, dst_mode, op_code, memory, memory_index, IC);

    /* The rest */
    encode_word(src, dst, src_mode, dst_mode, memory_index, memory, IC, both_registers, label_head, unknown_label_head,
                line, line_number);
    /* TODO: Create function that gets - the argument, the address_mode and create a code accordingly */
    /* TODO: check if the address_mode is DIRECTIVE (label) -> implement Algo 3 here */
}

void
encode_first_word(char *src, char *dst, addressMode src_mode, addressMode dst_mode, short op_code,
                  codeWord memory[MAX_MEMORY_SIZE],
                  int *memory_index, unsigned short *IC) {
    codeWord code;
    char address_mode_map = 0;

    init_code_word(&code);
    /* Set opcode */
    set_opcode_binary(&code, op_code);

    /* Set address mode map */
    address_mode_map = encode_address_mode_map(src_mode, dst_mode);

    /* Setting the address modes.*/
    code.bits[0] |= (address_mode_map << 3);
    code.bits[1] |= (address_mode_map >> 5) & 7;

    /* A,R,E - First words contains A always */
    set_are_field_binary(&code, A);

    /* Storing the code */
    memory[*memory_index] = code;
    printf("%.05d\n", convert_binary_to_octal(memory[*memory_index]));
    (*memory_index)++;
    (*IC)++;

}

void encode_word(char *src, char *dst, addressMode src_mode, addressMode dst_mode, int *memory_index,
                 codeWord memory[MAX_MEMORY_SIZE],
                 unsigned short *IC, bool both_registers, Node **label_head, Node **unknown_label_head, char *line,
                 int line_number) {
    codeWord code;
    init_code_word(&code);

    /* Handle when both arguments are registers */
    if (both_registers == true) {
        code = encode_both_registers(src, dst, src_mode, dst_mode);
        memory[*memory_index] = code;
        (*memory_index)++;
        (*IC)++;
    } else {
        if (src != NULL) {
            if (src_mode != DIRECT)
                code = encode_argument(src, src_mode, true);
            else
                code = encode_label(src, memory_index, IC, label_head, unknown_label_head, line, line_number);
            memory[*memory_index] = code;
            printf("%.05d\n", convert_binary_to_octal(memory[*memory_index]));
//            print_bits(memory[*memory_index]);
            (*memory_index)++;
            (*IC)++;
        }
        if (dst != NULL) {
            if (dst_mode != DIRECT)
                code = encode_argument(dst, dst_mode, false);
            else
                code = encode_label(dst, memory_index, IC, label_head, unknown_label_head, line, line_number);
            memory[*memory_index] = code;
            printf("%.05d\n", convert_binary_to_octal(memory[*memory_index]));
//            print_bits(memory[*memory_index]);
            (*memory_index)++;
            (*IC)++;
        }
    }
}


codeWord encode_both_registers(char *src, char *dst, addressMode src_mode, addressMode dst_mode) {
    /* Code for the src register */
    codeWord code1;
    /* Code for the dst register */
    codeWord code2;
    /* Code for combining both with bitwise OR */
    codeWord code;

    init_code_word(&code1);
    init_code_word(&code2);
    init_code_word(&code);

    code1 = encode_argument(src, src_mode, true);
    code2 = encode_argument(dst, dst_mode, false);
    code = bitwise_or_codes(code1, code2);

    return code;

}

codeWord encode_argument(char *arg, addressMode mode, bool is_src) {
    char *ptr = NULL;
    int number;
    codeWord code;
    init_code_word(&code);
    /* Encode immediate */
    if (mode == IMMEDIATE) {
        ptr = strchr(arg, '#');
        ptr++;
        number = atoi(ptr);
        convert_to_binary(number, &code, OTHER);


    }/* Encode indirect reg  (*r2) or direct reg (r2) */
    else {
        ptr = strchr(arg, 'r');
        ptr++;
        number = atoi(ptr);
        if (is_src == true)
            convert_to_binary(number, &code, SRC);
        else
            convert_to_binary(number, &code, DST);
    }

    /* Direct (labels) is encoded in a different function */
    set_are_field_binary(&code, A);
    return code;

}

codeWord
encode_label(char *arg, int *memory_index, unsigned short *IC, Node **label_head, Node **unknown_label_head, char *line,
             int line_number) {
    Node *temp = NULL;
    labelNode *found = NULL;
    codeWord code;

    init_code_word(&code);
    /* TODO: search the node */
    /* TODO: if the label is extern -> encode with E and return the code and store info inside the unknown */
    /* TODO: just add the information to the linked list*/

    return code;
}

char encode_address_mode_map(addressMode src, addressMode dst) {
    char map = 0;
    /* Checking if the address modes aren't init which means there aren't any arguments */
    if (dst != INIT)
        map |= 1 << dst;
    if (src != INIT)
        map |= 1 << (src + 4);
    return map;
}

void set_opcode_binary(codeWord *word, short opcode) {
    word->bits[1] |= (opcode << 3);
}

void set_are_field_binary(codeWord *code, areField field) {
    code->bits[0] |= (field << 0);
}
