/* File to configure the data structures for the project. */
/* Date of creation - 10/7/24 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "data_structures.h"
#include "utils.h"

Node *create_node(char *macr_name, char *macr_content, int line_number) {
    Node *new_node = NULL;
    new_node = generalized_malloc(sizeof(Node));

    /* Checking if the given name of the macro is legal. */
    if (is_macro_name_legal(macr_name) == true) {
        /* TODO: generate error - illegal name */
        free(new_node);
        return NULL;
    }
        /* Setting the attributes of the new node to the given arguments. */
    else {
        new_node->macr_name = macr_name;
        new_node->marc_content = macr_content;
        new_node->line_number = line_number;
        new_node->next = NULL;
        return new_node;
    }
}

Node *search_node(Node *head, char *macr_name, int *found) {
    Node *temp = head;
    /* Case 1: list is empty - Head is NULL. */
    if (head == NULL)
        return NULL;

    /* Iterating the linked list */
    while (temp != NULL) {
        if (strcmp(temp->macr_name, macr_name) == 0) {
            /* TODO: generate error - macro already in list */
            printf("Error\n");
            *found = true;
            return temp;
        }
        temp = temp->next;
    }
    /* Case 3: Couldn't find the given macro - not in the list. */
    return NULL;

}

void add_node(Node **head_ptr, char *name, char *content, int line_number) {
    int found;
    Node *new_node = NULL, *temp = NULL;
    found = false;

    temp = search_node(*head_ptr, name, &found);

    /* TODO: Check if the given content of the macro isn't the same as the already found macro content. */

    if (found == false) {
        new_node = create_node(name, content, line_number);
        /* The list is empty. */
        if (*head_ptr == NULL)
            *head_ptr = new_node;
            /* The marco isn't in the list. */
        else if (temp == NULL) {
            temp = *head_ptr;
            while (temp->next != NULL)
                temp = temp->next;
            temp->next = new_node;
        }
    }
}

void free_node(Node *node) {
    free(node->macr_name);
    free(node->marc_content);
    free(node);
}