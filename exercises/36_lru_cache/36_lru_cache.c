#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * 16 LRU 缓存淘汰算法（哈希表 + 双向链表）
 *  - put(k,v)、get(k) 均为 O(1)
 *  - 容量满时淘汰最久未使用（LRU）的元素
 */

typedef struct LRUNode {
    int key;
    int value;
    struct LRUNode* prev;
    struct LRUNode* next;
} LRUNode;

typedef struct HashEntry {
    int key;
    LRUNode* node;
    struct HashEntry* next;
} HashEntry;

typedef struct {
    int capacity;
    int size;
    LRUNode* head; /* 最近使用（MRU） */
    LRUNode* tail; /* 最久未使用（LRU） */
    /* 简单链式哈希表 */
    size_t bucket_count;
    HashEntry** buckets;
} LRUCache;

//FIXME: 自定义 写个最简单的哈希函数

static unsigned hash_int(int key, size_t bucket_count) {
    unsigned int k = (unsigned int)(key < 0 ? -key : key);
    return k % bucket_count;
}

static HashEntry* hash_find(LRUCache* c, int key, HashEntry*** pprev_next) {
    // TODO: 在这里添加你的代码
    if (!c)
    {
        return NULL;
    }
  
    // 查找哈希表中的键值对
    HashEntry** link = &c->buckets[hash_int(key, c->bucket_count)];
    HashEntry* p = c->buckets[hash_int(key, c->bucket_count)];
    while (p != NULL) 
    {
        if (p->key == key) {
            if(pprev_next != NULL)
            {
                *pprev_next = link;
            }
            return p;
        }
        link = &p->next;
        p = p->next;
    }
    if(pprev_next != NULL)
    {
        *pprev_next = link;
    }
    return NULL;
}

static void list_add_to_head(LRUCache* c, LRUNode* node) {
    // TODO: 在这里添加你的代码
    if (c == NULL || node == NULL)
    {
        return;
    }
    if (c->head == NULL)
    {
        c->head = node;
        c->tail = node;
        node->prev = NULL;
        node->next = NULL;
        return;
    }
    node->prev = NULL;
    node->next = c->head;
    c->head->prev = node;
    c->head = node;
}

static void list_remove(LRUCache* c, LRUNode* node) {
    // TODO: 在这里添加你的代码
    if (c == NULL || node == NULL)
    {
        return;
    }
    if (c->tail == NULL)
    {
        return;
    }
    if(node->prev == NULL)
    {
        c->head = node->next;
    }
    else
    {
        node->prev->next = node->next;
    }

    if(node->next == NULL)
    {
        c->tail = node->prev;
    }
    else
    {
        node->next->prev = node->prev;
    }

    node->next = NULL;
    node->prev = NULL;
    
}

static void list_move_to_head(LRUCache* c, LRUNode* node) {
    // TODO: 在这里添加你的代码
    if (c == NULL || node == NULL)
    {
        return;
    }
    list_remove(c, node);
    list_add_to_head(c, node);
}

static LRUNode* list_pop_tail(LRUCache* c) {
    // TODO: 在这里添加你的代码
    if (c == NULL || c->tail == NULL)
    {
        return NULL;
    }
    LRUNode* node = c->tail;
    list_remove(c, node);
    return node;
}

/* LRU 接口实现 */
static LRUCache* lru_create(int capacity) {
    // TODO: 在这里添加你的代码
    if (capacity <= 0)
    {
        return NULL;
    }
    LRUCache* c = malloc(sizeof(LRUCache));
    if (!c)
    {
        return NULL;
    }
    c->capacity = capacity;
    c->size = 0;
    c->head = NULL;
    c->tail = NULL;
    c->bucket_count = capacity;
    c->buckets = calloc(c->bucket_count, sizeof(HashEntry*));
    if (!c->buckets)
    {
        free(c);
        return NULL;
    }
    return c;
}

static void lru_free(LRUCache* c) {
    // TODO: 在这里添加你的代码
    if (!c)
    {
        return;
    }
    while (c->tail != NULL)
    {
        LRUNode* node = list_pop_tail(c);
        free(node);
    }
    
    for (int i = 0; i < c->bucket_count; i++) {
        HashEntry* p = c->buckets[i];
        while (p != NULL) {
            HashEntry* next = p->next;
            free(p);
            p = next;
        }
    }
    free(c->buckets);
    free(c);
}

static int lru_get(LRUCache* c, int key, int* out_value) {
    // TODO: 在这里添加你的代码
    if (!c || !out_value)
    {
        return 0;
    }
    HashEntry* p = hash_find(c, key, NULL);
    if (p == NULL)
    {
        return 0;
    }
    *out_value = p->node->value;
    list_move_to_head(c, p->node);
    return 1;
}

static void lru_put(LRUCache* c, int key, int value) {
    // TODO: 在这里添加你的代码
    if (!c) return;

    HashEntry** insert_pos = NULL;
    HashEntry* hit = hash_find(c, key, &insert_pos);
    if (hit) {
        hit->node->value = value;
        list_move_to_head(c, hit->node);
        return;
    }

    if (c->size == c->capacity) {
        LRUNode* victim = list_pop_tail(c);
        if (victim) {
            HashEntry** victim_pos = NULL;
            HashEntry* victim_entry = hash_find(c, victim->key, &victim_pos);
            if (victim_entry && victim_pos) {
                *victim_pos = victim_entry->next; // 从哈希链摘掉
                free(victim_entry);
            }
            free(victim);
            c->size--;
        }
    }

    LRUNode* node = malloc(sizeof(*node));
    if (!node) return;
    node->key = key;
    node->value = value;
    node->prev = node->next = NULL;
    list_add_to_head(c, node);

    HashEntry* e = malloc(sizeof(*e));
    if (!e) {
        list_remove(c, node);
        free(node);
        return;
    }
    e->key = key;
    e->node = node;
    e->next = *insert_pos;
    *insert_pos = e;
    c->size++;
    
    
}

/* 打印当前缓存内容（从头到尾） */
static void lru_print(LRUCache* c) {
    LRUNode* p = c->head;
    int first = 1;
    while (p) {
        if (!first) printf(", ");
        first = 0;
        printf("%d:%d", p->key, p->value);
        p = p->next;
    }
    printf("\n");
}

int main(void) {
    /* 容量 3：put(1,1), put(2,2), put(3,3), put(4,4), get(2), put(5,5) */
    LRUCache* c = lru_create(3);
    if (!c) {
        fprintf(stderr, "创建 LRU 失败\n");
        return 1;
    }

    lru_put(c, 1, 1); /* 缓存：1 */
    lru_put(c, 2, 2); /* 缓存：2,1 */
    lru_put(c, 3, 3); /* 缓存：3,2,1 (满) */
    lru_put(c, 4, 4); /* 淘汰 LRU(1)，缓存：4,3,2 */

    int val;
    if (lru_get(c, 2, &val)) {
        /* 访问 2：缓存：2,4,3 */
        (void)val; /* 演示无需使用 */
    }

    lru_put(c, 5, 5); /* 淘汰 LRU(3)，缓存：5,2,4 */

    /* 期望最终键集合：{2,4,5}，顺序无关。此处按最近->最久打印：5:5, 2:2, 4:4 */
    lru_print(c);

    lru_free(c);
    return 0;
}
