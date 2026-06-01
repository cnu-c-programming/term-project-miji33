#include <stdio.h>
#include "student.h"

int add_stu(Student **head, int id, char *name, int score) {
    // 인자 부족을 어떻게 아는데? 마지막 인자가 score 이니까 이게 NULL인지 판별하면 되나?
    // 비숫자/0/음수 ID, 점수 범위 초과, 비숫자 점수 -> "Error" 포함 메시지
    // 반복분 -> 중복 ID -> "Error: duplicate ID"

    Student newStudent = {id, name, score, NULL};
    if (*head == NULL) {
        *head = &newStudent;
        return 1;
    }

    newStudent.next = *head;
    *head = &newStudent;
    return 1;
}

int delete_stu(Student **head, int id) {
    // id가 비숫자/0/음수 -> "Error" 포함 메시지
    
    if (*head == NULL) return 0;    // "Error: student not found."

    Student *p = *head;

    if (p->id == id) {
        p->next = p->next->next;
            return 1;   // "Student deleted."
    }

    while (p->next != NULL) {  // next의 id가 id랑 다른 동안
        if (p->next->id == id) {
            p->next = p->next->next;
            return 1;   // "Student deleted."
        } else {
            p = p->next;
        }
    }

    return 0;   // "Error: student not found."
}

int update_stu(Student **head, int id, int score) {
    // 잘못된 점수, 점수 범위 오류 -> "Error" 포함 메시지
    // id가 이상할 경우는 생각 안하나? 그것도 생각해야할 것 같은데

    Student *p = *head;
    while (p != NULL) {  // next의 id가 id랑 다른 동안
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
    // id가 비숫자/0/음수 -> "Error" 포함 메시지

    Student *p = head;
    while (p != NULL) {  // next의 id가 id랑 다른 동안
        if (p->id == id) {
            return p;
        } else {
            p = p->next;
        }
    }

    return 0;   // "Error: student not found."
}

// 수정할거 많음. 위에 코드 다시 수정할 것.

void list_stu(Student *head) {

}

void stats_stu(Student *head) {

}

void exit_stu(Student *head) {

}