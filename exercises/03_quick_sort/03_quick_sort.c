#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 100
#define NAME_LEN     50

typedef struct {
    char name[NAME_LEN];
    int score;
} Student;

Student students[MAX_STUDENTS];

void swap(Student *a, Student *b)
{
    Student tmp = *a;
    *a = *b;
    *b = tmp;
}

void print_students(int l, int r)
{
    for(int i = l; i <= r; i++)
    {
        printf("%s %d\n", students[i].name, students[i].score);
    }
}

int sort(int l, int r)
{
    Student pivot = students[l];
    int i = l;
    int j = r;

    while (i < j) {
        while (i < j && students[j].score < pivot.score) {
            j--;
        }

        while (i < j && students[i].score >= pivot.score) {
            i++;
        }

        if (i < j) {
            swap(&students[i], &students[j]);
        }
    }

    swap(&students[l], &students[i]);
    return i;
}

void quick_sort(int l, int r) {
    if (l >= r)
        return;

    int p = sort(l, r);
    quick_sort(l, p - 1);
    quick_sort(p + 1, r);
}

int main(void) {
    FILE *file = fopen("03_students.txt", "r");
    if (!file) {
        printf("错误：无法打开文件 03_students.txt\n");
        return 1;
    }

    int n;
    fscanf(file, "%d", &n);

    if (n <= 0 || n > MAX_STUDENTS) {
        printf("学生人数无效：%d（应为 1-%d）\n", n, MAX_STUDENTS);
        fclose(file);
        return 1;
    }

    for (int i = 0; i < n; i++) {
        fscanf(file, "%s %d", students[i].name, &students[i].score);
    }
    fclose(file);

    quick_sort(0, n - 1);

    // 输出结果
    printf("\n快速排序后按成绩从高到低排序的学生名单：\n");
    for (int i = 0; i < n; i++) {
        printf("%s %d\n", students[i].name, students[i].score);
    }

    return 0;
}
