#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int (*CompareFunc)(const void *, const void *);

int compareInt(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

int compareFloat(const void *a, const void *b) {
    float diff = (*(float*)a - *(float*)b);
    return (diff > 0) ? 1 : ((diff < 0) ? -1 : 0);
}

int compareString(const void *a, const void *b) {
    return strcmp(*(char**)a, *(char**)b);
}

void sort(void *array, size_t n, size_t size, CompareFunc compare) {
    qsort(array, n, size, compare);
}

void processFile(const char *filename) {
    FILE *fin = fopen(filename, "r");
    if (!fin) {
        printf("错误: 无法打开文件 %s\n", filename);
        return;
    }

    int choice, n;
    if (fscanf(fin, "%d", &choice) != 1 || fscanf(fin, "%d", &n) != 1) {
        printf("错误: 文件 %s 格式不正确\n", filename);
        fclose(fin);
        return;
    }

    if (n > 20) n = 20;  // 最多支持20个元素

    printf("=== 处理数据来自: %s ===\n", filename);

    switch (choice) {
        // TODO: 在这里添加你的代码
        case 1: // 整数排序s
        {
            int *ints = malloc(n * sizeof(int));
            if(!ints)
            {
                printf("内存分配失败\n");
                fclose(fin);
                return;
            }
            for(int i = 0; i < n; i++)
            {
                if(fscanf(fin, "%d", &ints[i]) != 1)
                {
                    printf("读取文件失败\n");
                    free(ints);
                    fclose(fin);
                    return;
                }
            }
            sort(ints, n, sizeof(int), compareInt);
            for(int i = 0; i < n; i++)
            {
                printf("%d ", ints[i]);
            }
            printf("\n");
            free(ints);
            break;
        }
        case 2: // 浮点数排序s
        {
            float *floats = malloc(n * sizeof(float));
            if(!floats)
            {
                printf("内存分配失败\n");
                fclose(fin);
                return;
            }
            for(int i = 0; i < n; i++)
            {
                if(fscanf(fin, "%f", &floats[i]) != 1)
                {
                    printf("读取文件失败\n");
                    free(floats);
                    fclose(fin);
                    return;
                }
            }
            sort(floats, n, sizeof(float), compareFloat);   
            for(int i = 0; i < n; i++)
            {
                printf("%f ", floats[i]);
            }
            printf("\n");
            free(floats);
            break;
        }
        case 3: // 字符串排序s
        {
            enum { MAX_STRING_LEN = 128 };
            // 🔥 flam: char ** 可以理解成“字符串列表”，这里先申请 n 个 char* 指针格子
            char **strings = calloc(n, sizeof(char*));
            if(!strings)
            {
                printf("内存分配失败\n");
                fclose(fin);
                return;
            }
            for(int i = 0; i < n; i++)
            {
                // 🔥 flam: 每个 strings[i] 还要单独申请“真正放字符串的房间”
                strings[i] = malloc(MAX_STRING_LEN);
                if(!strings[i])
                {
                    printf("内存分配失败\n");
                    for(int j = 0; j < i; j++)
                    {
                        free(strings[j]);
                    }
                    free(strings);
                    fclose(fin);
                    return;
                }
                // 🔥 flam: strings[i] 先分配缓冲区，再限制 fscanf 最大读取长度
                if(fscanf(fin, "%127s", strings[i]) != 1)
                {
                    printf("读取文件失败\n");
                    for(int j = 0; j <= i; j++)
                    {
                        free(strings[j]);
                    }
                    free(strings);
                    fclose(fin);
                    return;
                }
            }
            // 🔥 flam: qsort 排的是指针数组，字符串内容不搬家，只调整 strings[i] 的顺序
            sort(strings, n, sizeof(char*), compareString);
            for(int i = 0; i < n; i++)
            {
                printf("%s ", strings[i]);
            }
            printf("\n");
            // 🔥 flam: 先释放每个字符串房间，再释放外层指针列表
            for(int i = 0; i < n; i++)
            {
                free(strings[i]);
            }
            free(strings);
            break;
        }
        default:
            printf("错误: 未知排序类型 %d\n", choice);
            fclose(fin);
            return;
    }

    fclose(fin);
}

int main() {
    processFile("int_sort.txt");
    processFile("float_sort.txt");

    return 0;
}
