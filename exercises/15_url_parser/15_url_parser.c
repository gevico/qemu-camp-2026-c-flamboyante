#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/**
 * URL参数解析器
 * 输入：包含http/https超链接的字符串
 * 输出：解析出所有的key-value键值对，每行显示一个
 */

int parse_url(const char* url) {
    int err = 0;

    // TODO: 在这里添加你的代码
    //主要思路，由于是const 我们这里尽量避免改动字符串，用strchr 不用 strtok
    const char *pre = strchr(url, '?');
    if(pre == NULL)
    {
        printf("error url\n");
        err = -1;
        goto exit;
    }
    const char *begin = pre + 1;
    while (*begin != '\0')
    {
        const char *end1 = strchr(begin, '=');
        if(end1 == NULL)
        {
            printf("error url\n");
            err = -2;
            goto exit;
        }

        const char *end2 = strchr(end1 + 1, '&');
        if (end2 == NULL) {
            end2 = begin + strlen(begin);
        }

        int key_len = (int)(end1 - begin);
        int value_len = (int)(end2 - end1 - 1);
        printf("key = %.*s, value = %.*s\n", key_len, begin, value_len, end1 + 1);

        if (*end2 == '&') {
            begin = end2 + 1;
        } else {
            break;
        }
    }
    return 0;

exit:
    return err;
}

int main() {
    const char* test_url = "https://cn.bing.com/search?name=John&age=30&city=New+York";

    printf("Parsing URL: %s\n", test_url);
    printf("Parameters:\n");

    parse_url(test_url);

    return 0;
}
