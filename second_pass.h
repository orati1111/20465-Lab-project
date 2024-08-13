/* Header file for the second pass. */
/* Date of creation - 13/8/24 */

#ifndef SECOND_PASS_H
#define SECOND_PASS_H

#include "utils.h"

/**
 * Function that starts the second pass for the project.
 * @param raw_file_name - The raw file name.
 * @param unknown_labels_head - Double pointer to the head of the unknown labels during the first pass.
 * @param label_head - Double pointer to the head of the labels list to search for the labels.
 * @param memory - The memory.
 * @param IC - Pointer to the command counter.
 * @param DC - Pointer to the instruction/data counter.
 * @param first_pass_error - bool that represents if an first_pass_error occurred in the first pass.
 * @param memory_ic_index - Pointer to the memory location of the commands codes.
 * @param memory_dc_index - Pointer to the memory location of the instruction codes.
 */
void
start_second_pass(char *raw_file_name, Node **unknown_labels_head, Node **label_head, codeWord memory[MAX_MEMORY_SIZE],
                  unsigned short *IC,
                  unsigned short *DC, bool first_pass_error, int *memory_ic_index, int *memory_dc_index);

/**
 * Function that updates the addresses of the labels according to their type (command,instruction)
 * @param label_head - Pointer to the head of the label list
 * @param IC - The command counter.
 * @param DC - The data/instruction counter.
 */
void update_label_address(Node *label_head, unsigned short IC, unsigned short DC);

/**
 * Function that iterates the unknown labels and stores inside the memory if needed.
 * @param unknown_label_head - Double pointer to the head of the unknown labels list.
 * @param label_head - Double pointer to the head of the labels list.
 * @param memory - The memory
 * @return NO_ERROR if succeeded, error_number otherwise.
 */
int iterate_unknown_labels(Node **unknown_label_head, Node **label_head, codeWord memory[MAX_MEMORY_SIZE]);

/**
 * Function that writes the object file.
 * @param obj_file - Pointer to the object file.
 * @param memory - The memory.
 * @param memory_ic_index - The memory location of the commands codes.
 * @param memory_dc_index - The memory location of the instruction codes.
 * @param IC - Pointer to the command counter.
 * @param DC - Pointer to the instruction/data counter.
 */
void write_object_file(FILE *obj_file, codeWord memory[MAX_MEMORY_SIZE], int memory_ic_index, int memory_dc_index,
                       unsigned short *IC,
                       unsigned short *DC);

#endif
