#include "mysed.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int parse_replace_command(const char* cmd, char** old_str, char** new_str) {
    // 检查输入参数有效性
    if (!cmd || !old_str || !new_str) {
        return -1;
    }
    
    // 初始化输出参数
    *old_str = NULL;
    *new_str = NULL;
    
    // TODO: 在这里添加你的代码
    // flam: 这里按 16 题的切片逻辑直接拆 s/old/new/ 这三段。
    const char *first_slash_begin = &cmd[2];
    const char *first_slash_end = strchr(first_slash_begin, '/');
    if (first_slash_end == NULL) {
        return -1;
    }

    const char *second_slash_begin = first_slash_end + 1;
    const char *second_slash_end = strchr(second_slash_begin, '/');
    if (second_slash_end == NULL) {
        return -1;
    }

    if ((*old_str = malloc((size_t)(first_slash_end - first_slash_begin) + 1)) == NULL) {
        return -1;
    }
    memcpy(*old_str, first_slash_begin, (size_t)(first_slash_end - first_slash_begin));
    (*old_str)[first_slash_end - first_slash_begin] = '\0';

    if ((*new_str = malloc((size_t)(second_slash_end - second_slash_begin) + 1)) == NULL) {
        free(*old_str);
        *old_str = NULL;
        return -1;
    }
    memcpy(*new_str, second_slash_begin, (size_t)(second_slash_end - second_slash_begin));
    (*new_str)[second_slash_end - second_slash_begin] = '\0';

    return 0;
}

void replace_first_occurrence(char* str, const char* old, const char* new) {
    // 检查输入参数有效性
    if (!str || !old || !new) {
        return;
    }
    
    // TODO: 在这里添加你的代码
    // flam: 这里直接沿用 16 题的第一次匹配替换逻辑。
    char *found = strstr(str, old);
    if (found == NULL) {
        return;
    }

    size_t len_old = strlen(old);
    size_t len_new = strlen(new);
    memmove(found + len_new, found + len_old, strlen(found + len_old) + 1);
    memcpy(found, new, len_new);
}

int __cmd_mysed(const char* rules, const char* str) {
    // 检查输入参数有效性
    if (!rules || !str) {
        fprintf(stderr, "Error: NULL rules or str parameter\n");
        return 1;
    }

    printf("rules: %s\n", rules);
    printf("str: %s\n", str);

    char* old_str = NULL;
    char* new_str = NULL;

    // 解析规则，例如 "s/old/new/"
    if (parse_replace_command(rules, &old_str, &new_str) != 0) {
        fprintf(stderr, "Invalid replace command format. Use 's/old/new/'\n");
        return 1;
    }

    if (!old_str || !new_str) {
        fprintf(stderr, "Error: Failed to parse old/new strings from rules\n");
        free(old_str);
        free(new_str);
        return 1;
    }

    // 复制原始字符串，因为我们可能会修改它（避免修改输入参数）
    char line[1024];
    strncpy(line, str, sizeof(line) - 1);
    line[sizeof(line) - 1] = '\0';  // 确保终止

    // 执行替换：在 line 中把 old_str 替换为 new_str（第一次出现）
    replace_first_occurrence(line, old_str, new_str);

    // 输出结果（建议加上换行，除非 str 本身带了）
    printf("%s\n", line);

    // 释放动态分配的内存
    free(old_str);
    free(new_str);

    return 0;
}
