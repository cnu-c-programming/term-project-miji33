#include <stdio.h>
#include <string.h>
#include "file_io.h"

int save(Student *head, const char *fp) {
    FILE* f = fopen(fp, "w");
    if (f == NULL) return -1;

    int cnt = 0;
    
    fprintf(f, "id,name,score\n");
    while (head != NULL) {
        fprintf(f, "%d,%s,%d\n", head->id, head->name, head->score);
        head = head->next;
        cnt++;
    }

    fclose(f);
    return cnt;
}

int reload(Student **head, const char *fp) {
    free_stu(*head);
    *head = NULL;
    
    FILE* f = fopen(fp, "r");
    if (f == NULL) return -1;

    int cnt = 0;
    int id; char name[32]; int score;

    char line[128];
    fgets(line, sizeof(line), f);
    if (strncmp(line, "id,name,score", 13) != 0) {
        fclose(f);
        return 0;
    }
    
    while (fscanf(f, "%d,%31s,%d\n", &id, name, &score) == 3) {
        add_stu(head, id, name, score);
        cnt++;
    }
    
    fclose(f);
    return cnt;
}