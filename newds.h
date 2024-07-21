/* Data structures header file. */
/* Date of creation - 10/7/24 */

#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include "utils.h"

/*
 * Struct that represents the node and its attributes:
 * macr_name - The name of the macro.
 * macr_content - The content inside the macro.
 * next - Pointer to the next node in the list.
 */
typedef struct Node {
    void *data;
    nodeType node_type;
    struct Node *next;
} Node;

/*
 * Struct that represents the content of a macro node.
  * macr_name - The name of the macro.
 *  macr_content - The content inside the macro.
 */
typedef struct macro_node {
    char *macr_name;
    char *macr_content;
} macroNode;

/*
 * Struct that represents the label node.
 * label_name - The name of the label to store.
 * address - The memory address of the label (starts at 100)
 */
typedef struct label_node {
    char *label_name;
    int address;
    labelType label_type;
} labelNode;

/*
 * Structure that represents the 16-bits of a word.
 * 0 - 2 bits - A,R,E Field.
 * 3 - 6 bits - Destination Operand.
 * 7 - 10 bits - Source Operand.
 * 11 - 14 bits - The binary representation of the Opcode (0-15).
 */
typedef struct code_word {
    unsigned char bits[2];
} codeWord;

/*
 * Function that creates the node of the linked list.
 * @param data - Pointer to the data of the node to create
 * @param size_of_data - The size of the data
 * @param node_type - The type of the node (Macro,Label)
 * @return The node that was created.
 */
Node *create_node(void *data, size_t size_of_data, nodeType node_type);

/*
 * Function that searches a node by its name.
 * @param head - Pointer to the head of the list.
 * @param name - The name to find in the linked list.
 * @param node_type - The type of the node to find.
 * @param found - Pointer to a flag that represents a node had been found.
 * @return Node, NULL otherwise.
 */
Node *search_node(Node *head, char *name, nodeType node_type, int *found);

/*
 * Function that adds a node to the list.
 * @param head - Double pointer to the head of the list.
 * @param node - Pointer to the node to add.
 */
void add_node(Node **head, Node *node);

/*
 * Function that frees the all the nodes and the content inside each node.
 * @param head - Pointer to the head of the list.
 * @param node_type - The type of nodes in the list.
 * @param content_only - A flag to specify to free only the macro content.
 */
void free_list(Node *head, nodeType node_type, bool content_only);

/*
 * Function that displays the list of macros node by node.
 * @param head - Pointer to the head of the list.
 */
void print_macr_list(Node *head);

/*
 * Function that displays the list of labels node by node.
 * @param head - Pointer to the head of the list.
 */
void print_label_list(Node *head);

/*
 * Function that initializes all the bits to zero.
 * @param code_word - Pointer to the code word to initialize;
 */
void init_code_word(codeWord *word);

/*
 * Function that prints the word bit by bit.
 * @param word - The word to print.
 */
void print_bits(codeWord word);


#endif
