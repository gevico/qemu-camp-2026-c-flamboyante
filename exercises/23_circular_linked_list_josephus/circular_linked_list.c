#include "circular_linked_list.h"

#include <stdio.h>
#include <stdlib.h>

Node* create_circular_list(int n) {
    // TODO: 在这里添加你的代码
    if (n <= 0) {
        return NULL;
    }
    Node* head = malloc(sizeof(Node));
    head->id = 1;
    Node* node = head;
    for(int i = 1; i < n; i++) 
    {
        node->next = malloc(sizeof(Node));
        node = node->next;
        node->id = i+1;
        node->next = NULL;
    }
    node->next = head;
    return head;
}   

void free_list(Node* head) {
    // TODO: 在这里添加你的代码
    if (head == NULL)
    {
        return;
    }
    Node* node = head;
    while (node->next != head)
    {
        Node* next = node->next;
        free(node);
        node = next;
    }
    free(head);
    
}
