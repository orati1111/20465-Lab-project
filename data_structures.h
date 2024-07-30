/* Data structures header file. */
/* Date of creation - 10/7/24 */

#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include "globals.h"

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
 * label_type - The type of the label.
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
typedef struct codeWord {
    unsigned char bits[2];
} codeWord;


/*
 * Structure that represents the parts of an instruction in the code.
 *
 */
typedef struct instr {
    char *label;
} instrParts;

/*
 * Structure that represents the parts of a command in the code.
 */

typedef struct command {
    char * label;
    char * op_names;
} commandParts;


/*
 * Function that creates the node of the linked list.
 * @param data - Pointer to the data of the node to create
 * @param size_of_data - The size of the data
 * @param node_type - The type of the node (Macro,Label)
 * @return The node that was created.
 */
Node *create_node(void *data, size_t size_of_data, nodeType node_type);

/*
 * Function that creates the macro node itself.
 * @param macro_name - The name of the macro.
 * @param macro_content - The content of the macro.
 * @return Pointer to the created node.
 */
macroNode *create_macro_node(char *macro_name, char *macro_content);

/*
 * Function that creates the label node itself.
 * @param label_name - The name of the label.
 * @param address - The address in memory of the label.
 * @param type - The type of label (extern, entry etc.)
 * @return Pointer to the created node.
 */
labelNode *create_label_node(char *label_name, int address, labelType type);

/*
 * Function that searches a node by its name.
 * @param head - Pointer to the head of the list.
 * @param name - The name to find in the linked list.
 * @param node_type - The type of the node to find.
 * @return Node, NULL otherwise.
 */
Node *search_node(Node *head, char *name, nodeType node_type);

/*
 * Function that adds a node to the list.
 * @param head - Double pointer to the head of the list.
 * @param node - Pointer to the node to add.
 */
void add_node(Node **head, Node *node);

/*
 * Function that frees the all the nodes and the content inside each node.
 * @param head - Double pointer to the head of the list.
 * @param node_type - The type of nodes in the list.
 * @param type - A flag to specify what to free
 */
void free_list(Node **head, nodeType node_type, freeType type);

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
