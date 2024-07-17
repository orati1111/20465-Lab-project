/* File to configure the data structures for the project. */
/* Date of creation - 10/7/24 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "data_structures.h"
#include "utils.h"


Node *create_node(char *macr_name, char *macr_content, int start_decl_line) {
    Node *new_node = NULL;
    new_node = generalized_malloc(sizeof(Node));
    /* Setting the attributes of the new node to the given arguments. */
    new_node->macr_name = macr_name;
    new_node->marc_content = macr_content;
    new_node->start_decl_line = start_decl_line;
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

void add_node(Node **head_ptr, char *name, char *content, int start_decl_line) {
    Node *new_node = NULL, *temp = NULL;
    temp = *head_ptr;
    new_node = create_node(name, content, start_decl_line);
    /* The list is empty. */
    if (*head_ptr == NULL)
        *head_ptr = new_node;
        /* Otherwise */
    else {
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = new_node;
    }
   /* log_print_list(head_ptr); */

}


void free_node(Node *node) {
    Node *temp = node;
    while (temp != NULL) {
        free(temp->macr_name);
        free(temp->marc_content);
        temp = temp->next;
    }
    while (node != NULL) {
        temp = node;
        node = node->next;
        free(temp);
    }
}
void log_print_list(Node ** head){
    Node *temp;
    temp = *head;
    while(temp != NULL){
        printf("\nStart at:%d\nMacro name: '%s'\n",temp->start_decl_line,temp->macr_name);
        printf("Macro content: '%s'",temp->marc_content);
        temp = temp->next;
    }
    printf("\n\n---END OF LIST---\n\n");
}