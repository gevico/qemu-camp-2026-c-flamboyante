#include "simple_tree.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

Queue* create_queue() {
    Queue *q = (Queue*)malloc(sizeof(Queue));
    q->front = q->rear = NULL;
    return q;
}

void enqueue(Queue *q, TreeNode *tree_node) {
    // TODO: 在这里添加你的代码
    if(tree_node == NULL)
    {
        return;
    }
    QueueNode *new_node = (QueueNode*)malloc(sizeof(QueueNode));
    new_node->tree_node = tree_node;
    new_node->next = NULL;
    if(q->front == NULL)
    {
        q->front = q->rear = new_node;
    }
    else
    {
        q->rear->next = new_node;
        q->rear = new_node;
    }
}

TreeNode* dequeue(Queue *q) {
    // TODO: 在这里添加你的代码
    if(q == NULL || q->front == NULL)
    {
        return NULL;
    }
    TreeNode *tree_node = q->front->tree_node;
    QueueNode *temp = q->front;
    q->front = q->front->next;

    if(q->front == NULL)
    {
        q->rear = NULL;
    }
    free(temp);
    return tree_node;
}

bool is_empty(Queue *q) {
    return q->front == NULL;
}

void free_queue(Queue *q) {
    while (!is_empty(q)) {
        dequeue(q);
    }
    free(q);
}

TreeNode* build_tree_by_level(int *level_order, int size) {
    // TODO: 在这里添加你的代码
    if (size <= 0 || level_order == NULL)
    {
        return NULL; 
    }

    TreeNode *root = (TreeNode*)malloc(sizeof(TreeNode));
    root->val = level_order[0];
    root->left = NULL;
    root->right = NULL;

    Queue *q = create_queue();
    if(q == NULL)
    {
        free(root);
        return NULL;
    }
    enqueue(q, root);

    int i = 1;
    while(!is_empty(q) && i < size)
    {
        TreeNode *now_root = dequeue(q);
        if (now_root == NULL)
        {
            break;
        }
        if (level_order[i] != INT_MIN)
        {
            TreeNode *left_node = (TreeNode*)malloc(sizeof(TreeNode));
            left_node->val = level_order[i];
            left_node->left = NULL;
            left_node->right = NULL;
            now_root->left = left_node;
            enqueue(q, left_node);
        }
        i++;
        if (i >= size)
        {
            break;
        }
        if (level_order[i] != INT_MIN)
        {
            TreeNode *right_node = (TreeNode*)malloc(sizeof(TreeNode));
            right_node->val = level_order[i];
            right_node->left = NULL;
            right_node->right = NULL;
            now_root->right = right_node;
            enqueue(q, right_node);
        }
        i++;
        if (i >= size)
        {
            break;
        }
        
        
    }
    free_queue(q);
   
    return root;
}

void preorder_traversal(TreeNode *root) {
    // TODO: 在这里添加你的代码
    if (root == NULL)
    {
        return;
    }
    printf("%d ", root->val);
    preorder_traversal(root->left);
    preorder_traversal(root->right);
}

void preorder_traversal_iterative(TreeNode *root) {
    // TODO: 在这里添加你的代码
    if (root == NULL)
    {
        return;
    }

    // 简单动态栈（容量按需扩展）
    int cap = 16;
    int top = 0;
    TreeNode **stack = (TreeNode **)calloc(cap, sizeof(TreeNode *));
    if (stack == NULL) {
        return;
    }
    stack[top++] = root;

    while(top > 0)
    {
        TreeNode *now_root = stack[--top];
        if (now_root == NULL)
        {
            continue;
        }
        printf("%d ", now_root->val);
        if (now_root->right != NULL)
        {
            if (top == cap) 
            {
                cap *= 2;
                TreeNode **tmp = (TreeNode **)realloc(stack, sizeof(TreeNode *) * cap);
                if (tmp == NULL) {
                free(stack);
                return;
                }
                stack = tmp;
            }
            stack[top++] = now_root->right;
        }
        if (now_root->left != NULL)
        {
            if (top == cap) 
            {
                cap *= 2;
                TreeNode **tmp = (TreeNode **)realloc(stack, sizeof(TreeNode *) * cap);
                if (tmp == NULL) {
                free(stack);
                return;
                }
                stack = tmp;
            }
            
            stack[top++] = now_root->left;
        }
    }

    free(stack);

}

void free_tree(TreeNode *root) {
    if (root == NULL) {
        return;
    }
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}
