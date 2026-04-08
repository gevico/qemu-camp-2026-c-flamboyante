#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char id[20];
    char name[50];
    int age;
} Student;

int main() {
    FILE *file = fopen("students.txt", "r");
    if (file == NULL) {
        printf("无法打开文件\n");
        return 1;
    }
    
    Student *students[3];
    
    for (int i = 0; i < 3; i++) 
    {
	    // TODO: 在这里添加你的代码
        students[i] = (Student *)malloc(sizeof(Student));
        // 动态内存申请后必须判空，避免解引用空指针
        if (students[i] == NULL) {
            printf("内存分配失败\n");
            for (int j = 0; j < i; j++) {
                free(students[j]);
            }
            fclose(file);
            return 1;
        }

        // 检查读取结果，确保每行都成功读取 3 个字段
        if (fscanf(file, "%19s %49s %d",
                  students[i]->id,
                  students[i]->name,
                  &students[i]->age) != 3) {
            printf("文件格式错误\n");
            for (int j = 0; j <= i; j++) {
                free(students[j]);
            }
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    
    for (int i = 0; i < 3; i++) 
    {
        printf("学号：%s, 姓名：%s, 年龄：%d\n", 
               students[i]->id, 
               students[i]->name, 
               students[i]->age);
    }
    
    for (int i = 0; i < 3; i++) 
    {
        free(students[i]);
    }
    
    return 0;
}
