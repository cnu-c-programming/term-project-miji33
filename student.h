#pragma once
#include <stdio.h>

typedef struct Student {
    int id;
    char name[32];
    int score;
    struct Student* next;
} Student;

int add_stu(Student **head, int id, char *name, int score);
int delete_stu(Student **head, int id);
int update_stu(Student **head, int id, int score);
Student *find_stu(Student *head, int id);
void list_stu(Student *head);
void stats_stu(Student *head);

void exit_stu(Student *head);