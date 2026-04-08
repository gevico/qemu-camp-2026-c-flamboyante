#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ARGS 10
#define MAX_INPUT_LENGTH 256

int shell_parse(char *buf, char *argv[]);
void execute_command(int argc, char *argv[]);

// in:  format -> hello world "hello world" 1234567
// Multiple command parameters entered from the command line (number of parameters does not exceed 10)
// out:  format -> Parameter X: Content, Length: X
// Parse the command parameters through a character pointer array, and sequentially display the content and length of each parameter on the screen
//
int main(void)
{
    FILE *file;
    char input[MAX_INPUT_LENGTH];
    char *argv[MAX_ARGS + 1] = {NULL};
    int argc = 0;

    file = fopen("command_file.txt", "r");
    if (!file) {
        fprintf(stderr, "❌ Error: Cannot open input file '%s'\n", "command_file.txt");
        return 1;
    }

    printf("✅ Reading commands from '%s':\n\n", "command_file.txt");

    while (fgets(input, MAX_INPUT_LENGTH, file) != NULL) {
        input[strcspn(input, "\n")] = '\0';

        if (strlen(input) == 0 || strspn(input, " \t") == strlen(input)) {
            continue;
        }

        printf("➡️  Input: %s\n", input);

        argc = shell_parse(input, argv);

        if (argc == 0) {
            printf("⚠️  No valid command parsed.\n\n");
            continue;
        }

        execute_command(argc, argv);
        printf("\n");
    }

    fclose(file);
    return 0;
}

// shell_parse 和 execute_command 保持不变
int shell_parse(char *buf, char *argv[])
{
    int argc = 0;
    int state = 0;
    // state: 0=空闲(不在参数中), 1=普通参数, 2=双引号参数
    // TODO: 在这里添加你的代码，完成命令行解析
    // 功能：将输入字符串buf按空格分割成多个参数，存入argv数组
    // 返回：参数个数argc
    // 提示：使用状态机的方式处理，注意处理字符串结束符
    for (int i = 0; buf[i] != '\0'; i++) {
        char c = buf[i];
        int is_sep = (c == ' ' || c == '\t' || c == '\n');

        if (state == 0) {
            if (is_sep) {
                continue;
            }
            if (argc >= MAX_ARGS) {
                break;
            }
            if (c == '"') {
                // 引号参数：argv 指向引号后的第一个字符
                argv[argc++] = &buf[i + 1];
                state = 2;
            } else {
                // 普通参数：argv 指向当前字符
                argv[argc++] = &buf[i];
                state = 1;
            }
        } else if (state == 1) {
            if (is_sep) {
                // 普通参数结束，原地写 '\0' 分割字符串
                buf[i] = '\0';
                state = 0;
            }
        } else if (state == 2) {
            if (c == '"') {
                // 引号参数结束，去掉右引号
                buf[i] = '\0';
                state = 0;
            }
        }
    }
    // 兼容 argv 约定：最后一个指针置 NULL
    argv[argc] = NULL;

    return argc;
}

void execute_command(int argc, char *argv[])
{
    printf("Parsing result: Total %d parameters\n", argc);

    for (int i = 0; i < argc; i++) {
        printf("Parameter %d: Content: %s, Length: %zu\n",
               i + 1, argv[i], strlen(argv[i]));
    }

    if (strcmp(argv[0], "help") == 0) {
        printf("This is help command\n");
    } else if (strcmp(argv[0], "echo") == 0) {
        printf("Echo: ");
        for (int i = 1; i < argc; i++) {
            printf("%s ", argv[i]);
        }
        printf("\n");
    } else if (strcmp(argv[0], "add") == 0 && argc == 3) {
        int a = atoi(argv[1]);
        int b = atoi(argv[2]);
        printf("%d + %d = %d\n", a, b, a + b);
    } else {
        printf("Unknown command: %s\n", argv[0]);
    }
}
