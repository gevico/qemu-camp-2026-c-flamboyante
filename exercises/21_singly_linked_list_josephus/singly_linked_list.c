#include "singly_linked_list.h"

#include <stdio.h>
#include <stdlib.h>

// 全局头指针
static link head = NULL;

// 创建新节点
link make_node(unsigned char item) {
    // TODO: 在这里添加你的代码
    link new_node = malloc(sizeof(struct node));
    if (new_node == NULL) {
        printf("[ERR] : %s-%d: malloc failed\n", __func__, __LINE__);
        return NULL;
    }
    new_node->item = item;
    new_node->next = NULL;
    return new_node;
}

// 释放节点
void free_node(link p) { free(p); }

// 查找节点
link search(unsigned char key) {
    // TODO: 在这里添加你的代码
    for (link s = head; s != NULL; s = s->next) {
        if (s->item == key) {
            return s;
        }
    }
    printf("[WARN] : %s-%d: key not found\n", __func__, __LINE__);
    return NULL;
}

// 在链表头部插入节点
void insert(link p) {
    // TODO: 在这里添加你的代码
    p->next = head;
    head = p;
}

// 删除指定节点
void delete(link p) {
    // TODO: 在这里添加你的代码
    if (p == head) {
        head = head->next;
    } else {
        for (link s = head; s != NULL; s = s->next) {
            if (s->next == p) {
                s->next = p->next;
                break;
            }
        }
    }
    free_node(p);
}

// 遍历链表
void traverse(void (*visit)(link)) {
    // TODO: 在这里添加你的代码
    for (link s = head; s != NULL; s = s->next) {
        visit(s);
    }
}

// 销毁整个链表
void destroy(void) {
    // TODO: 在这里添加你的代码
    while (head != NULL) {
        link p = head;
        head = head->next;
        free_node(p);
    }
}

// 在链表头部推入节点
void push(link p) { 
    // TODO: 在这里添加你的代码
    insert(p);
 }

// 从链表头部弹出节点
link pop(void) {
    // TODO: 在这里添加你的代码
    if (head == NULL) {
        printf("[WARN] : %s-%d: list is empty\n", __func__, __LINE__);
        return NULL;
    }
    link p = head;
    head = head->next;
    free_node(p);
    return p;
}

// 释放链表内存
void free_list(link list_head) {
    // TODO: 在这里添加你的代码
    while (list_head != NULL) {
        link p = list_head;
        list_head = list_head->next;
        free_node(p);
    }
}
