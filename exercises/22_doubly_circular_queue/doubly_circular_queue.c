#include "doubly_circular_queue.h"

#include <stdlib.h>

// 头尾哨兵
static struct node tailsentinel;
static struct node headsentinel = {0, NULL, &tailsentinel};
static struct node tailsentinel = {0, &headsentinel, NULL};

static link head = &headsentinel;
static link tail = &tailsentinel;

// 创建新节点, 并初始化节点数据, 不初始化 prev 和 next 指针
link make_node(int data) {
    // TODO: 在这里添加你的代码
    link new_node = malloc(sizeof(struct node));
    if (new_node == NULL) {
        return NULL;
    }
    new_node->data = data;
    new_node->prev = NULL;
    new_node->next = NULL;
    return new_node;
}
//释放指定指针对应的节点内存
void free_node(link p) {
    // TODO: 在这里添加你的代码
    if (p == NULL) return;
    free(p);
}
//双向链表内查找指定数据的节点指针
link search(int key) {
    // TODO: 在这里添加你的代码
    for (link s = head->next; s != tail; s = s->next) {
        if (s->data == key) {
            return s;
        }
    }
    return NULL;
}

void insert(link p) {
    // TODO: 在这里添加你的代码
    if (p == NULL) {
        return;
    }
    p->prev = head;
    head->next->prev = p;
    p->next = head->next;
    head->next = p;
}
//从链表中移除指定结点（不释放）
void delete(link p) {
    // TODO: 在这里添加你的代码
    if (p == NULL || p == head || p == tail) {
        return;
    }
    p->prev->next = p->next;
    p->next->prev = p->prev;
    p->prev = NULL;
    p->next = NULL;
}

void traverse(void (*visit)(link)) {
    // TODO: 在这里添加你的代码
    if (visit == NULL) {
        return;
    }
    for (link s = head->next; s != tail; s = s->next) {
        visit(s);
    } 
}

void destroy(void) {
    // TODO: 在这里添加你的代码
    link s = head->next;
    while (s != tail) {
        link next = s->next;
        free_node(s);
        s = next;
    }
    head->next = tail;
    tail->prev = head;
}
