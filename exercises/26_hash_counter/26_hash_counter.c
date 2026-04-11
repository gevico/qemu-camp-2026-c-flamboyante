#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TABLE_SIZE 1024  // 哈希表大小

// 哈希表节点结构
typedef struct HashNode {
    char *word;
    int count;
    struct HashNode *next;
} HashNode;

// 哈希表结构
typedef struct {
    HashNode **table;
    int size;
} HashTable;

// djb2哈希函数
unsigned long djb2_hash(const char *str) {
    // TODO: 在这里添加你的代码
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;  // hash * 33 + c
    }
    return hash;
}

// 创建哈希表
HashTable *create_hash_table(int size) {
    HashTable *ht = malloc(sizeof(HashTable));
    ht->table = calloc(size, sizeof(HashNode *));
    ht->size = size;
    return ht;
}

// 向哈希表中插入单词 
void hash_table_insert(HashTable *ht, const char *word) {
    if (word == NULL || *word == '\0') {
        return;
    }

    unsigned long hash = djb2_hash(word) % ht->size;

    // TODO: 在这里添加你的代码
    if (ht->table[hash] == NULL)
    {
        HashNode *insert = malloc(sizeof(HashNode));
        if (insert == NULL) {
            return;
        }
        // ✅ 修复点：check 用 -std=c11 编译，避免使用非标准 strdup
        insert->word = malloc(strlen(word) + 1);
        if (insert->word == NULL) {
            free(insert);
            return;
        }
        strcpy(insert->word, word);
        insert->count = 1;
        insert->next = NULL;
        ht->table[hash] = insert;
        return;
    }
    else
    {
        HashNode *node = ht->table[hash];
        while (1)
        {
            if (strcmp(node->word, word) == 0)
            {
                node->count++;
                return;
            }
            if (node->next != NULL)
                node = node->next;
            else
            {
                HashNode *insert = malloc(sizeof(HashNode));
                if (insert == NULL) {
                    return;
                }
                // ✅ 修复点：同上，手动拷贝字符串
                insert->word = malloc(strlen(word) + 1);
                if (insert->word == NULL) {
                    free(insert);
                    return;
                }
                strcpy(insert->word, word);
                insert->count = 1;
                insert->next = NULL;
                node->next = insert;
                return;
            }
        }
    }
}

// 从哈希表中获取所有单词及其计数
void get_all_words(HashTable *ht, HashNode **nodes, int *count) {
    // TODO: 在这里添加你的代码
    if (ht == NULL || nodes == NULL || count == NULL)
    {
        return;
    }
    
    for (int i = 0; i < ht->size; i++)
    {
        HashNode *node = ht->table[i];
        while (node != NULL)
        {
            // ✅ 修复点：先用 *count 作为下标，再递增计数器
            nodes[*count] = node;
            (*count)++;
            node = node->next;
        }
    }
    
}

// 比较函数用于排序
int compare_nodes(const void *a, const void *b) {
    HashNode *node_a = *(HashNode **)a;
    HashNode *node_b = *(HashNode **)b;
    
    // 先按计数降序，再按字母升序
    // TODO: 在这里添加你的代码
    if (node_a->count == node_b->count)
    {
        return strcmp(node_a->word, node_b->word);
    }
    
    return node_b->count - node_a->count;
}

// 释放哈希表内存
void free_hash_table(HashTable *ht) {
    for (int i = 0; i < ht->size; i++) {
        HashNode *node = ht->table[i];
        while (node != NULL) {
            HashNode *temp = node;
            node = node->next;
            free(temp->word);
            free(temp);
        }
    }
    free(ht->table);
    free(ht);
}

// 从字符串中获取下一个单词
char *get_next_word(const char **text) {
    // TODO: 在这里添加你的代码
    if (text == NULL || *text == NULL || **text == '\0')
    {
        return NULL;
    }
    while (**text != '\0' && !isalpha((unsigned char)**text))
    {
        (*text)++;
    }
    if (**text == '\0') {
        return NULL;
    }
    const char *start = *text;
    const char *end = start;
    while (*end != '\0' && isalpha((unsigned char)*end))
    {
        end++;
    }
     // 计算单词长度
    int len = (int)(end - start);
    if (len == 0) {
        return NULL;
    }
    
    // 分配内存并复制
    char *word = malloc(len + 1);
    if (word == NULL) {
        return NULL;
    }
    // ✅ 修复点：提取时统一转小写，保证统计不区分大小写
    for (int i = 0; i < len; i++) {
        word[i] = (char)tolower((unsigned char)start[i]);
    }
    word[len] = '\0';
    
    // ✅ 修复点：这里应停在 end，下一轮再跳过分隔符，避免漏词
    *text = end;

    return word;
}

int main(int argc, char *argv[]) {
    const char* file_path = "paper.txt";

    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        perror("无法打开文件");
        return 1;
    }

    HashTable *ht = create_hash_table(TABLE_SIZE);
    char buffer[4096];
    
    printf("正在读取文件: %s\n", file_path);
    
    // 从文件读取直到EOF
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        const char *ptr = buffer;
        char *word;
        
        while ((word = get_next_word(&ptr)) != NULL) {
            printf("%s\n", word);
            hash_table_insert(ht, word);
            free(word);
        }
    }
    
    fclose(file);
    
    HashNode **nodes = malloc(TABLE_SIZE * sizeof(HashNode *));
    int node_count = 0;
    get_all_words(ht, nodes, &node_count);
    
    // 排序
    qsort(nodes, node_count, sizeof(HashNode *), compare_nodes);
    
    // 输出结果
    printf("\n单词统计结果（按频率降序排列）:\n");
    printf("%-20s %s\n", "单词", "出现次数");
    printf("-------------------- ----------\n");
    for (int i = 0; i < node_count; i++) {
        printf("%s:%d\n", nodes[i]->word, nodes[i]->count);
    }
    
    // 释放内存
    free(nodes);
    free_hash_table(ht);
    
    return 0;
}
