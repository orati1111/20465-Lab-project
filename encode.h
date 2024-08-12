/* encode header file */
/* Date of creation - 11/8/24 */

#ifndef ENCODE_H
#define ENCODE_H

#include "utils.h"

/**
 * Function that gets an instruction and encodes to bits.
 * @param instr - Pointer to the instruction struct to encode.
 * @param memory - Pointer to the memory array.
 * @param memory_index - Pointer to the index inside the memory array.
 * @param DC - Pointer to the DC variable.
 */
void encode_instruction(instrParts *instruction, codeWord *memory, int *memory_index, unsigned short *DC);

/**
 * Function that gets a command and encodes to bits.
 * @param command - Pointer to the command struct to encode.
 * @param memory - Pointer to the memory array.
 * @param memory_index - Pointer to the index inside the memory array.
 * @param IC - Pointer to the IC variable.
 * @param label_head - Double pointer to the head of the label list.
 * @param unknown_label_head - Double pointer to the unknown label list.
 * @param line - The line that was encoded (For error display only).
 * @param line_number - The number of the line (For error only).
 */
void encode_command(commandParts *command, codeWord memory[MAX_MEMORY_SIZE], int *memory_index, unsigned short *IC,
                    Node **label_head, Node **unknown_label_head, char *line, int line_number);

/**
 * Function that encodes the first word in each command.
 * @param src - The source operand argument.
 * @param dst - The destination operand argument
 * @param src_mode - The address mode for the source.
 * @param dst_mode - The address mode for the destination.
 * @param op_code - The op_code of the command.
 * @param memory - Pointer to the memory array.
 * @param memory_index - Pointer to the index inside the memory array.
 * @param IC - Pointer to the IC variable.
 */
void
encode_first_word(char *src, char *dst, addressMode src_mode, addressMode dst_mode, short op_code,
                  codeWord memory[MAX_MEMORY_SIZE],
                  int *memory_index, unsigned short *IC);


/**
 * Function that encodes given arguments to a codeWord and stores it.
 * @param src - The source operand argument.
 * @param dst - The destination operand argument
 * @param src_mode - The address mode for the source.
 * @param dst_mode - The address mode for the destination.
 * @param memory_index - Pointer to the index inside the memory array.
 * @param memory - Pointer to the memory array.
 * @param IC - Pointer to the IC variable.
 * @param both_registers - Flag that represents that both arguments are registers (encode in one word).
 * @param label_head - Double pointer to the head of the label list.
 * @param unknown_label_head - Double pointer to the unknown label list.
 * @param line - The line that was encoded (For error display only).
 * @param line_number - The number of the line (For error only).
 */
void encode_word(char *src, char *dst, addressMode src_mode, addressMode dst_mode, int *memory_index,
                 codeWord memory[MAX_MEMORY_SIZE],
                 unsigned short *IC, bool both_registers, Node **label_head, Node **unknown_label_head, char *line,
                 int line_number);

/**
 * Function that encodes 2 arguments that are both registers as one codeWord.
 * @param src - The source operand argument.
 * @param dst - The destination operand argument
 * @param src_mode - The address mode for the source.
 * @param dst_mode - The address mode for the destination.
 * @return The encoded word.
 */
codeWord encode_both_registers(char *src, char *dst, addressMode src_mode, addressMode dst_mode);


/**
 * Function that encodes a given argument (excluding labels that were given as arguments)
 * @param arg - The argument given.
 * @param mode - The address_mode of the given argument.
 * @param is_src - Flag that represents if the given argument is the src or dst in the command.
 * @return The encoded word.
 */
codeWord encode_argument(char *arg, addressMode mode, bool is_src);

/**
 * Function that encodes the label only.
 * If the label that was given as an argument isn't declared yet , it gets stored inside the unknown_label list for the second pass.
 * Extern labels that were already declared as extern get encoded and added to the unknown label for the .ext file.
 * @param arg - The argument given.
 * @param memory_index - Pointer to the index inside the memory array.
 * @param IC - Pointer to the IC variable.
 * @param label_head - Double pointer to the head of the label list.
 * @param unknown_label_head - Double pointer to the unknown label list.
 * @param line - The line that was encoded (For error display only).
 * @param line_number - The number of the line (For error only).
 * @return codeWord if it succeeded, 0 otherwise.
 */
codeWord
encode_label(char *arg, int *memory_index, unsigned short *IC, Node **label_head, Node **unknown_label_head, char *line,
             int line_number);

/**
 * Function that creates a map of 8 bits for the src address_mode and dst address_mode.
 * bits 0-3 for dst address mode, bits 4-7 are for src address mode.
 * @param src - src address mode
 * @param dst - dst address mode
 * @return The mapping.
 */
char encode_address_mode_map(addressMode src, addressMode dst);

/**
 * Function that converts the operation code into binary and stores it inside the appropriate place
 * in the code_word.
 * @param word - Pointer to the code_word to set the bits in.
 * @param opcode - The operation code.
 */
void set_opcode_binary(codeWord *word, short opcode);

/**
 * Function that sets the first 3 bits of a codeword according to the A,R,E provided.
 * @param code - Pointer to the code.
 * @param field - A,R,E
 */
void set_are_field_binary(codeWord *code, areField field);



#endif
