#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 1024

int parse_replace_command(const char* cmd, char** old_str, char** new_str) {
    if (cmd[0] != 's' || cmd[1] != '/') {
        return -1;
    }

    // TODO: 在这里添加你的代码
    const char *first_slash_begin = &cmd[2];
    const char *first_slash_end = strchr(first_slash_begin, '/');
    if(first_slash_end == NULL)
    {
        return -1;
    }

    const char *second_slash_begin = first_slash_end + 1;
    const char *second_slash_end = strchr(second_slash_begin, '/');
    if(second_slash_end == NULL)
    {
        return -1;
    }
    
    if((*old_str = malloc(first_slash_end - first_slash_begin + 1)) == NULL)
    {
        return -1;
    }
    memcpy(*old_str, first_slash_begin, first_slash_end - first_slash_begin);
    (*old_str)[first_slash_end - first_slash_begin] = '\0';
    
    if((*new_str = malloc(second_slash_end - second_slash_begin + 1)) == NULL)
    {
        free(*old_str);
        return -1;
    }
    memcpy(*new_str, second_slash_begin, second_slash_end - second_slash_begin);
    (*new_str)[second_slash_end - second_slash_begin] = '\0';
    
    return 0;
}

void replace_first_occurrence(char* str, const char* old, const char* new) {
    char* found = strstr(str, old);
    if (found == NULL) {
        return;
    }
    size_t len_old = strlen(old);
    size_t len_new = strlen(new);
    memmove(found + len_new, found + len_old, strlen(found + len_old) + 1);
    memcpy(found, new, len_new);
}


int main(int argc, char* argv[]) {
    const char* replcae_rules = "s/unix/linux/";

    char line[MAX_LINE_LENGTH] = {"unix is opensource. unix is free os."};

    char* old_str = NULL;
    char* new_str = NULL;
    
    if (parse_replace_command(replcae_rules, &old_str, &new_str) != 0) {
        fprintf(stderr, "Invalid replace command format. Use 's/old/new/'\n");
        return 1;
    }

    
    replace_first_occurrence(line, old_str, new_str);
    fputs(line, stdout);

    free(old_str);
    free(new_str);
    return 0;
}
