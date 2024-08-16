/* File to configure the data structures for the project. */
/* Date of creation - 10/7/24 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"
#include "data_structures.h"


Node *create_node(void *data, size_t size_of_data, nodeType node_type) {
    Node *node = malloc(sizeof(Node));
    if (node == NULL)
        return NULL;
    node->data = malloc(size_of_data);
    if (node->data == NULL) {
        free(node);
        return NULL;
    }
    memcpy(node->data, data, size_of_data);
    node->node_type = node_type;
    node->next = NULL;
    return node;

}

macroNode *create_macro_node(char *macro_name, char *macro_content) {
    macroNode *temp = NULL;
    temp = malloc(sizeof(macroNode));
    if (temp == NULL)
        return NULL;
    temp->macr_name = strdupli(macro_name);
    if (temp->macr_name == NULL) {
        cleanup("m", temp);
        return NULL;
    }
    temp->macr_content = strdupli(macro_content);
    if (temp->macr_content == NULL) {
        cleanup("sm", temp->macr_name, temp);
        return NULL;
    }
    return temp;
}

labelNode *create_label_node(char *label_name, unsigned short address, labelType type, bool is_label_command,
                             bool entry_and_local) {
    labelNode *temp = NULL;
    temp = malloc(sizeof(labelNode));
    if (temp == NULL)
        return NULL;
    temp->label_name = strdupli(label_name);
    if (temp->label_name == NULL) {
        cleanup("l", temp);
        return NULL;
    }
    temp->address = address;
    temp->label_type = type;
    temp->is_label_command = is_label_command;
    temp->entry_and_local = entry_and_local;
    return temp;
}

unknownLabelNode *
create_unknown_label_node(char *label_name, int memory_index, char *line, int line_number, unsigned short IC) {
    unknownLabelNode *temp = NULL;
    temp = malloc(sizeof(unknownLabelNode));
    if (temp == NULL)
        return NULL;
    temp->label_name = strdupli(label_name);
    if (temp->label_name == NULL) {
        cleanup("u", temp);
        return NULL;
    }
    temp->line = strdupli(line);
    if (temp->line == NULL) {
        cleanup("su", temp->label_name, temp);
    }
    temp->memory_index = memory_index;
    temp->line_number = line_number;
    temp->IC = IC;

    return temp;
}

Node *search_node(Node *head, char *name, nodeType node_type) {
    Node *temp = head;
    macroNode *macro_node = NULL;
    labelNode *label_node = NULL;
    /* list is empty - Head is NULL. */
    if (head == NULL)
        return NULL;

    /* Iterating the linked list */
    while (temp != NULL) {
        /* Getting the data of the node. */
        /* The node to find is a macro */
        if (node_type == MACRO) {
            /* Casting the data to be a macro node */
            macro_node = (macroNode *) temp->data;
            /* Found a match */
            /* printf("MACRO NAME: '%s' , CHECKING: '%s'\n",macro_node->macr_name,name); */
            if (strcmp(macro_node->macr_name, name) == 0) {
                /* printf("MATCH\n"); */
                return temp;
            }

        }
        /* The node to find is a label */
        if (node_type == LABEL) {
            /* Casting the data to be a label node */
            label_node = (labelNode *) temp->data;
            /* Found a match */
            if (strcmp(label_node->label_name, name) == 0) {
                return temp;
            }

        }
        temp = temp->next;
    }
    /* Couldn't find the given macro - not in the list. */
    return NULL;
}


void add_node(Node **head_ptr, Node *node) {
    Node *temp = NULL;
    temp = *head_ptr;
    /* The list is empty. */
    if (*head_ptr == NULL)
        *head_ptr = node;
        /* Otherwise */
    else {
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = node;
    }
}

void free_list(Node **head, nodeType node_type, freeType type) {
    Node *temp = NULL;
    Node *next = NULL;
    macroNode *macro_node = NULL;
    labelNode *label_node = NULL;
    unknownLabelNode *unknown_label_node = NULL;

    temp = *head;

    while (temp != NULL) {
        /* Setting a pointer to the next node*/
        next = temp->next;
        /* In case the list is a macro list */
        if (node_type == MACRO) {
            macro_node = (macroNode *) temp->data;
            /* Free all the data in the nodes*/
            if (type == ALL) {
                if (macro_node->macr_name != NULL)
                    free(macro_node->macr_name);
            }
            /* Otherwise */
            if (macro_node->macr_content != NULL) {
                free(macro_node->macr_content);
                macro_node->macr_content = NULL;
            }
        }
            /* In case the list is a label list */
        else if (node_type == LABEL) {
            label_node = (labelNode *) temp->data;
            if (label_node->label_name != NULL) {
                free(label_node->label_name);
            }
        }
            /* In case the list is an unknown label list*/
        else {
            unknown_label_node = (unknownLabelNode *) temp->data;
            if (unknown_label_node->label_name != NULL)
                free(unknown_label_node->label_name);
            if (unknown_label_node->line != NULL)
                free(unknown_label_node->line);
        }
        /* Freeing each data and node */
        if (type == ALL) {
            if (temp->data != NULL)
                free(temp->data);
            free(temp);
            temp = next;
        } else
            temp = next;
    }
    /* Setting the head to NULL */
    if (type == ALL)
        *head = NULL;
}

void print_list(Node *head, nodeType type) {
    Node *temp = NULL;
    labelNode *label_node = NULL;
    macroNode *macro_node = NULL;
    unknownLabelNode *unknown_label_node = NULL;

    temp = head;

    /* The list given is a macro list */
    if (type == MACRO) {
        while (temp != NULL) {
            macro_node = (macroNode *) temp->data;
            printf("\nMacro name: '%s'\n", macro_node->macr_name);
            printf("Macro content: '%s'", macro_node->macr_content);
            temp = temp->next;
        }
        printf("\n\n---END OF LIST---\n\n");
    }
        /* The list given is a label list */
    else if (type == LABEL) {
        while (temp != NULL) {
            label_node = (labelNode *) temp->data;
            printf("\nLabel Name: '%s'\n", label_node->label_name);
            printf("Label address: '%d'\n", label_node->address);
            printf("Label type: '%d'\n", label_node->label_type);
            printf("Is command: '%d'", label_node->is_label_command);
            temp = temp->next;
        }
        printf("\n\n---END OF LIST---\n\n");
    }
        /* the list given is an Unknown label list*/
    else {
        while (temp != NULL) {
            unknown_label_node = (unknownLabelNode *) temp->data;
            printf("\nLabel Name: '%s'\n", unknown_label_node->label_name);
            printf("Label IC: '%d'\n", unknown_label_node->IC);
            printf("Memory index: '%d'\n", unknown_label_node->memory_index);
            printf("The line: '%s'", unknown_label_node->line);
            printf("The linen number '%d'", unknown_label_node->line_number);
            temp = temp->next;
        }
        printf("\n\n---END OF LIST---\n\n");
    }

}


void init_code_word(codeWord *word) {
    word->bits[0] = 0;
    word->bits[1] = 0;
}

void print_bits(codeWord word) {
    int i;
    int byte_index, bit_index, bit;
    for (i = SIZE_OF_WORD - 1; i >= 0; --i) {
        byte_index = i / 8;
        bit_index = i % 8;
        bit = (word.bits[byte_index] >> bit_index) & 1;
        printf("%d", bit);
    }
    printf("\n");
}

void init_struct_parts(instrParts *iptr, commandParts *cptr) {
    if (iptr != NULL) {
        iptr->label = NULL;
        iptr->type = NONE;
        iptr->length.string_length = 0;
        iptr->data.string = NULL;
    } else {
        cptr->label = NULL;
        cptr->op_code = 0;
        cptr->dst = NULL;
        cptr->src = NULL;
        cptr->dst_mode = INIT;
        cptr->src_mode = INIT;
        cptr->both_registers = false;
    }
}

void free_structs(instrParts *iptr, commandParts *cptr) {
    if (iptr != NULL) {
        if (iptr->type == DATA)
            free(iptr->data.numbers);
        if (iptr->type == STRING)
            free(iptr->data.string);
        free(iptr);
    } else {
        if (cptr->src != NULL)
            free(cptr->src);
        if (cptr->dst != NULL)
            free(cptr->dst);
        free(cptr);
    }
}

void init_memory(codeWord memory[MAX_MEMORY_SIZE]) {
    int i;
    for (i = 0; i < MAX_MEMORY_SIZE; i++) {
        init_code_word(&memory[i]);
    }
}

