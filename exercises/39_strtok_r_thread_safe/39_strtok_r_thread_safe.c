#include <stdio.h>
#include <stdlib.h>

/*
 * 19 字符串分割器（类似 strtok_r）
 * 目标：实现线程安全的字符串分割函数，不依赖静态全局状态
 * 原型：char* strtok_r(char *str, const char *delim, char **saveptr)
 * 示例：输入 "hello,world test"，分隔符 ", " → 依次得到 hello、world、test
 */

/* 判断字符 c 是否在分隔符集合 delim 中 */
static int is_delim(char c, const char *delim) {
    // TODO: 在这里添加你的代码
    if(strchr(delim, c) != NULL)
    {
        return 1;
    }
    return 0;
}

/* 线程安全版本：通过 saveptr 维护调用状态，不使用静态变量 */
char *strtok_r(char *str, const char *delim, char **saveptr) {
    // TODO: 在这里添加你的代码
    if(str == NULL && *saveptr == NULL)
    {
        return NULL;
    }
    char *pre_str = NULL;
    char *token = NULL;
    if(*saveptr == NULL)
    {
        *saveptr = str;
    }
    pre_str = *saveptr;

    int start = 0;
    int end = 0;
    while(is_delim(pre_str[start], delim))
    {
        start++;
    }

    for(end = start; end < strlen(pre_str); end++)
    {
        if(is_delim(pre_str[end], delim))
        {
            break;
        }
    }
    if(end == strlen(pre_str))
    {
        token = pre_str;
        *saveptr = NULL;
    }
    else
    {
        token = pre_str + start;
        pre_str[end] = '\0';
        *saveptr = pre_str + end + 1;
    }
    return token;
}

int main(void) {
    char buf[] = "hello,world test";
    const char *delim = ", ";
    char *save = NULL;

    char *tok = strtok_r(buf, delim, &save);
    while (tok) {
        printf("%s\n", tok);
        tok = strtok_r(NULL, delim, &save);
    }
    return 0;
}
