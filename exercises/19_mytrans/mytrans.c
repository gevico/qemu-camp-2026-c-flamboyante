// mytrans.c
#include "myhash.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void trim(char *str) {
    if (!str) {
      return;
    }
    // flam: 先原地移除前导空白，再移除尾部空白，避免空串越界。
    size_t start = 0;
    while (str[start] && isspace((unsigned char)str[start])) {
      start++;
    }
    if (start > 0) {
      memmove(str, str + start, strlen(str + start) + 1);
    }

    size_t len = strlen(str);
    while (len > 0 && isspace((unsigned char)str[len - 1])) {
      str[len - 1] = '\0';
      len--;
    }
}

int load_dictionary(const char *filename, HashTable *table,
                    uint64_t *dict_count) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    perror("无法打开词典文件");
    return -1;
  }

  char line[1024];
  char current_word[100] = {0};
  char current_translation[1024] = {0};
  int in_entry = 0;

    // TODO: 在这里添加你的代码
  while (fgets(line, sizeof(line), file)) {
    if(line[0] != '#')
      continue;
    char *key = line + 1;
    trim(key);
    strcpy(current_word, key);

    if(fgets(current_translation, sizeof(current_translation), file) == NULL)
      continue;
    char *ptr = strstr(current_translation, "Trans:");
    if(ptr == NULL)
      continue;
    char *value = ptr + strlen("Trans:");
    trim(value);
    hash_table_insert(table, current_word, value);
    (*dict_count)++;
  }

  fclose(file);
  return 0;
}
