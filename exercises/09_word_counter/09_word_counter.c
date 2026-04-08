#include <stdio.h>

int main() {
    char str[]="Don't ask what your country can do for you, but ask what you can do for your country.";
    int wordCount = 0;
    // TODO: 在这里添加你的代码
    int inWord = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] != ' ' && str[i] != '\n' && !inWord) {
            inWord = 1;
            wordCount++;
        } else if (str[i] == ' ' || str[i] == '\n') {
            inWord = 0;
        }
    }

    
    printf("单词数量: %d\n", wordCount);
    
    return 0;
}