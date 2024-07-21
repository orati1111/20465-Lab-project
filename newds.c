/* File to configure the data structures for the project. */
/* Date of creation - 10/7/24 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "newds.h"
#include "utils.h"


Node *create_node(void *data, size_t size_of_data, nodeType node_type) {
    Node *node = generalized_malloc(size_of_data);
    if (node == NULL)
        return NULL;
    node->data = generalized_malloc(sizeof data);
    if (node->data == NULL) {
        free(node);
        return NULL;
    }
    memcpy(node->data, data, size_of_data);
    node->node_type = node_type;
    node->next = NULL;
    return node;

}

Node *search_node(Node *head, char *name, nodeType node_type, int *found) {
    Node *temp = head;
    void *tmp_data;
    macroNode *macro_node = NULL;
    labelNode *label_node = NULL;
    /* list is empty - Head is NULL. */
    if (head == NULL)
        return NULL;

    /* Iterating the linked list */
    while (temp != NULL) {
        /* Getting the data of the node. */
        tmp_data = temp->data;
        /* The node to find is a macro */
        if (node_type == MACRO) {
            /* Casting the data to be a macro node */
            macro_node = (macroNode *) tmp_data;
            /* Found a match */
            if (strcmp(macro_node->macr_name, name) == 0) {
                *found = true;
                return temp;
            }

        }
        /* The node to find is a label */
        if (node_type == LABEL) {
            /* Casting the data to be a label node */
            label_node = (labelNode *) tmp_data;
            /* Found a match */
            if (strcmp(label_node->label_name, name) == 0) {
                *found = true;
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

void free_node(Node *head, nodeType node_type, bool content_only) {
    Node *temp = head;
    void *tmp_data;
    macroNode *macro_node = NULL;
    labelNode *label_node = NULL;

    if (node_type == MACRO) {
        tmp_data = temp->data;
        /* Free The content of each node */
        while (temp != NULL) {
            macro_node = (macroNode *) tmp_data;
            if (content_only == false)
                free(macro_node->macr_name);
            if (macro_node->macr_content != NULL)
                free(macro_node->macr_content);
            temp = temp->next;
        }
    }
    if (node_type == LABEL) {
        tmp_data = temp->data;
        /* Free The content of each node */
        while (temp != NULL) {
            label_node = (labelNode *) tmp_data;
            free(label_node->label_name);
            temp = temp->next;
        }
    }
    if (content_only == false) {
        /* At the end, free each node */
        while (head != NULL) {
            temp = head;
            head = head->next;
            free(temp->data);
            free(temp);
        }
    }
}


void print_macr_list(Node *head) {
    Node *temp;
    temp = head;
    void *tmp_data;
    macroNode *macro_node = (macroNode *) tmp_data;
    while (temp != NULL) {
        tmp_data = temp->data;
        printf("\nMacro name: '%s'\n", macro_node->macr_name);
        printf("Macro content: '%s'", macro_node->macr_content);
        temp = temp->next;
    }
    printf("\n\n---END OF LIST---\n\n");
}

void print_label_list(Node *head) {
    Node *temp;
    temp = head;
    void *tmp_data;
    labelNode *label_node = (labelNode *) tmp_data;
    while (temp != NULL) {
        tmp_data = temp->data;
        printf("\nLabel Name: '%s'\n", label_node->label_name);
        printf("Label address: '%d'\n", label_node->address);
        printf("Label type: '%d", label_node->label_type);
        temp = temp->next;
    }
    printf("\n\n---END OF LIST---\n\n");
}

void init_code_word(codeWord *word) {
    word->bits[0] = 0;
    word->bits[1] = 0;
}

void print_bits(codeWord word) {
    int i;
    int byte_index, bit_index, bit;
    for (i = 15; i >= 0; --i) {
        byte_index = i / 8;
        bit_index = i % 8;
        bit = (word.bits[byte_index] >> bit_index) & 1;
        printf("%d", bit);
    }
}