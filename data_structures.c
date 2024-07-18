/* File to configure the data structures for the project. */
/* Date of creation - 10/7/24 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "data_structures.h"
#include "utils.h"


Node *create_node(char *macr_name, char *macr_content) {
    Node *new_node = NULL;
    new_node = generalized_malloc(sizeof(Node));
    /* Setting the attributes of the new node to the given arguments. */
    new_node->macr_name = macr_name;
    new_node->macr_content = macr_content;
    new_node->next = NULL;
    return new_node;

}

Node *search_node(Node *head, char *macr_name, int *found) {
    Node *temp = head;
    /* Case 1: list is empty - Head is NULL. */
    if (head == NULL)
        return NULL;

    /* Iterating the linked list */
    while (temp != NULL) {
        if (strcmp(temp->macr_name, macr_name) == 0) {
            *found = true;
            return temp;
        }
        temp = temp->next;
    }
    /* Case 3: Couldn't find the given macro - not in the list. */
    return NULL;

}

void add_node(Node **head_ptr, char *macr_name, char *macr_content) {
    Node *new_node = NULL, *temp = NULL;
    temp = *head_ptr;
    new_node = create_node(macr_name, macr_content);
    /* The list is empty. */
    if (*head_ptr == NULL)
        *head_ptr = new_node;
    /* Otherwise */
    else {
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = new_node;
    }

}

void free_node(Node *node) {
    Node *temp = node;
    /* Free The content of each node */
    while (temp != NULL) {
        free(temp->macr_name);
        free(temp->macr_content);
        temp = temp->next;
    }
    /* At the end, free each node */
    while (node != NULL) {
        temp = node;
        node = node->next;
        free(temp);
    }
}

void log_print_list(Node **head) {
    Node *temp;
    temp = *head;
    while (temp != NULL) {
        printf("\nMacro name: '%s'\n",temp->macr_name);
        printf("Macro content: '%s'", temp->macr_content);
        temp = temp->next;
    }
    printf("\n\n---END OF LIST---\n\n");
}