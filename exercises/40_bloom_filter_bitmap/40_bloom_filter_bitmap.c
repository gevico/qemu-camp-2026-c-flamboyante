#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * 20 位图实现 Bloom 过滤器
 * 需求：m=100 位图，k=3 个哈希函数；插入 "apple"、"banana"，查询示例
 */

typedef struct {
    unsigned char *bits;
    size_t m; /* 位图大小（比特） */
} Bloom;

static Bloom *bloom_create(size_t m) {
    // TODO: 在这里添加你的代码
    Bloom *bf = malloc(sizeof(Bloom));
    if (!bf) {
        fprintf(stderr, "malloc failed\n");
        return NULL;
    }
    bf->bits = malloc(m / 8);
    if (!bf->bits) {
        fprintf(stderr, "malloc failed\n");
        free(bf);
        return NULL;
    }
    bf->m = m;
    return bf;
}

static void bloom_free(Bloom *bf) {
    // TODO: 在这里添加你的代码
    free(bf->bits);
    free(bf);
}

/* 位操作 */
static void set_bit(unsigned char *bm, size_t idx) {
    // TODO: 在这里添加你的代码
    bm[idx / 8] |= (1 << (idx % 8));
}
static int test_bit(const unsigned char *bm, size_t idx) {
    // TODO: 在这里添加你的代码
    return (bm[idx / 8] & (1 << (idx % 8))) != 0;
}

/* 三个简单哈希：sum(c*k) % m */
static size_t hash_k(const char *s, size_t m, int k) {
    // TODO: 在这里添加你的代码
    size_t h = k;  // 用 k 初始化，区分不同哈希函数
    while (*s) {
        h = (h * 131 + *s) % m;  // 131 是常用的乘数
        s++;
    }
    return h;
}

static void bloom_add(Bloom *bf, const char *s) {
    // TODO: 在这里添加你的代码
    for (int k = 0; k < 3; k++) {
        size_t h = hash_k(s, bf->m, k);
        set_bit(bf->bits, h);
    }
}

static int bloom_maybe_contains(Bloom *bf, const char *s) {
    // TODO: 在这里添加你的代码
    for (int k = 0; k < 3; k++) {
        size_t h = hash_k(s, bf->m, k);
        if (!test_bit(bf->bits, h)) {
            return 0;  // 有一个哈希函数返回 0，说明元素不存在
        }
    }
    return 1;  // 所有哈希函数都返回 1，说明元素可能存在
}

int main(void) {
    const size_t m = 100; /* 位图大小 */
    Bloom *bf = bloom_create(m);
    if (!bf) {
        fprintf(stderr, "bloom create failed\n");
        return 1;
    }

    /* 插入元素："apple"、"banana" */
    bloom_add(bf, "apple");
    bloom_add(bf, "banana");

    /* 查询元素："apple"（应存在）、"orange"（可能存在误判） */
    int apple = bloom_maybe_contains(bf, "apple");
    int grape = bloom_maybe_contains(bf, "grape"); /* 未插入，可能误判 */

    printf("apple exists: %d\n", apple); /* 必须为 1（无假阴性） */
    printf("grape exists: %d\n", grape); /* 允许 0 或 1（可能误判） */

    bloom_free(bf);
    return 0;
}
