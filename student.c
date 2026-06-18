#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "student.h"

int add_stu(Student **head, int id, char *name, int score) {
    Student *newStudent = malloc(sizeof(Student));
    if (newStudent == NULL) return 0;   // malloc 실패

    newStudent->id = id;
    strncpy(newStudent->name, name, 31);
    newStudent->score = score;
    newStudent->next = NULL;
    
    Student *p = *head;
    while(p != NULL) {
        if (p->id == id) {
            free(newStudent);
            return -1;
        }
        p = p->next;
    }

    if (*head == NULL) {
        *head = newStudent;
    } else {
        Student *p = *head;
        while (p->next != NULL) p = p->next;
        p->next = newStudent;
    }
    return 1;
}

int delete_stu(Student **head, int id) {
    if (*head == NULL) return 0;    // "Error: student not found."

    Student *p = *head;

    if (p->id == id) {
        *head = p->next;
        free(p);
        return 1;   // "Student deleted."
    }

    while (p->next != NULL) {
        if (p->next->id == id) {
            Student *q = p->next;
            p->next = q->next;
            free(q);
            return 1;   // "Student deleted."
        } else {
            p = p->next;
        }
    }

    return 0;   // "Error: student not found."
}

int update_stu(Student **head, int id, int score) {
    Student *p = *head;
    while (p != NULL) {
        if (p->id == id) {
            p->score = score;
            return 1;   // "Student updated."
        } else {
            p = p->next;
        }
    }

    return 0;   // "Error: student not found."
}

Student *find_stu(Student *head, int id) {
    Student *p = head;
    while (p != NULL) {
        if (p->id == id) {
            return p;
        } else {
            p = p->next;
        }
    }

    return NULL;   // "Error: student not found."
}

void list_stu(Student *head) {
    if (head == NULL) {
        printf("No students found.");
        return;
    }

    Student *p = head;
    printf("ID\tName\tScore\n");
    while(p != NULL) {
        printf("%d\t%s\t%d\n", p->id, p->name, p->score);
        p = p->next;
    }
}

void stats_stu(Student *head) {
    if (head == NULL) {
        printf("No student data available.");
        return;
    }

    Student *p = head;
    int cnt = 0;
    int sum = 0;
    int max = 0;
    int min = 100;
    while(p != NULL) {
        int score = p->score;
        cnt++;
        sum += score;
        max = (score > max) ? score : max;
        min = (score < min) ? score : min;
        p = p->next;
    }

    printf("Count: %d\n", cnt);
    printf("Average: %.1f\n", (float)sum/cnt);
    printf("Max: %d\n", max);
    printf("Min: %d\n", min);
}

void free_stu(Student *head) {
    if (head == NULL) {
        return;
    } else {
        Student *p = head->next;
        free(head);
        free_stu(p);
    }
}