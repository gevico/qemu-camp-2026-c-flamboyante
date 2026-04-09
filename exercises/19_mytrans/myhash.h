// hash_table.h
#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdint.h>
#define HASH_TABLE_SIZE 10000003  // 应选一个较大的质数，有利于分布

// 哈希节点
typedef struct HashNode {
    char *key;              // 英文单词，如 "a", "a.m."
    char *value;            // 中文翻译，如 "art. 一;字母A"
    struct HashNode *next;  // 用于解决冲突（链地址法）
} HashNode;

// 哈希表
typedef struct {
    HashNode *buckets[HASH_TABLE_SIZE];
} HashTable;

// 函数声明
/**
 * @brief 创建一个空的哈希表
 * 
 * @return HashTable* 指向新创建的哈希表的指针
 */
HashTable* create_hash_table();  
/**
 * @brief 释放哈希表占用的内存
 * 
 * @param table 指向要释放的哈希表的指针
 */
void free_hash_table(HashTable *table);  
/**
 * @brief 插入键值对到哈希表
 * 
 * @param table 指向要插入键值对的哈希表的指针
 * @param key 要插入的键（英文单词）
 * @param value 要插入的值（中文翻译）
 * @return int 1 成功，0 失败
 */
int hash_table_insert(HashTable *table, const char *key, const char *value);  
/**
 * @brief 查找哈希表中指定键的值
 * 
 * @param table 指向要查找的哈希表的指针
 * @param key 要查找的键（英文单词）
 * @return const char* 指向找到的值（中文翻译），或 NULL 如果未找到
 */
const char* hash_table_lookup(HashTable *table, const char *key);                
/**
 * @brief 从文件加载词典到哈希表
 * 
 * @param filename 词典文件的路径
 * @param table 指向要加载词典的哈希表的指针
 * @param dict_count 指向加载到词典中的单词数量的指针
 * @return int 0 成功，-1 失败
 */
int load_dictionary(const char *filename, HashTable *table, uint64_t* dict_count);

#endif
