#include <stdio.h>
#include "command.h"

const char *g_csv_path = NULL;
int num_commands;

#ifdef ADMIN_MODE
Command commands[] = {
    {"save", handle_save, "save", "Save students to CSV"},
    {"reload", handle_reload, "reload", "Reload students from CSV"},
    {"add", handle_add, "add <id> <name> <score>", "Add a student"},
    {"delete", handle_delete, "delete <id>", "Delete a student"},
    {"update", handle_update, "update <id> <score>", "Update student score"},
    {"find", handle_find, "find <id>", "Find student"},
    {"list", handle_list, "list", "List students"},
    {"stats", handle_stats, "stats", "Show statistics"},
    {"help", handle_help, "help", "Show help"},
    {"clear", handle_clear, "clear", "Clear screen"},
    {"exit", handle_exit, "exit", "Exit shell"}
};
int num_commands = sizeof(commands) / sizeof(commands[0]);
#endif

#ifdef CLIENT_MODE
Command commands[] = {
    {"reload", handle_reload, "reload", "Reload students from CSV"},
    {"find", handle_find, "find <id>", "Find student"},
    {"list", handle_list, "list", "List students"},
    {"stats", handle_stats, "stats", "Show statistics"},
    {"help", handle_help, "help", "Show help"},
    {"clear", handle_clear, "clear", "Clear screen"},
    {"exit", handle_exit, "exit", "Exit shell"}
};
int num_commands = sizeof(commands) / sizeof(commands[0]);
#endif

ShellResult handle_save(char* args, Student **head) {
    (void)args;
    int result = save(*head, g_csv_path);

    if (result == -1) {
        printf("Error: file open.\n");
        return SHELL_ERR_FILE_OPEN;
    } else {
        printf("Saved %d students to %s\n", result, g_csv_path);
        return SHELL_OK;
    }
}

ShellResult handle_reload(char* args, Student **head) {
    int result = reload(head, g_csv_path);

    if (result == -1) {
        printf("Error: file open.\n");
        return SHELL_ERR_FILE_OPEN;
    } else if (result == 0) {
        printf("Error: header\n");
        return SHELL_ERR_FILE_OPEN;
    } else {
        printf("Reloaded %d students from %s\n", result, g_csv_path);
        return SHELL_OK;
    }
}

ShellResult handle_add(char* args, Student **head) {
    int id; char name[32]; int score;
    int parse = sscanf(args, "%d %31s %d", &id, name, &score);
    if (parse != 3) {
        printf("Error: invalid argument.\n");
        return SHELL_ERR_INVALID_ARGUMENT;
    }

    if (id <= 0) {
        printf("Error: invalid ID.\n");
        return SHELL_ERR_INVALID_ARGUMENT;
    }

    if (!(0 <= score && score <= 100)) {
        printf("Error: invalid score.\n");
        return SHELL_ERR_INVALID_SCORE;
    }

    int result = add_stu(head, id, name, score);

    switch(result) {
        case 1: // 성공
            printf("Student added.\n");
            return SHELL_OK;
        case 0: // malloc 실패
            printf("Error: memory allocation failed.\n");
            return SHELL_ERR_MEMORY;
        case -1: // 중복 id
            printf("Error: duplicate ID.\n");
            return SHELL_ERR_DUPLICATE_STUDENT;
        default:
            return SHELL_ERR_INVALID_ARGUMENT;
    }
}

ShellResult handle_delete(char* args, Student **head) {
    int id;
    int parse = sscanf(args, "%d", &id);
    if (parse != 1) {
        printf("Error: invalid argument.\n");
        return SHELL_ERR_INVALID_ARGUMENT;
    }

    if (id <= 0) {
        printf("Error: invalid ID.\n");
        return SHELL_ERR_INVALID_ARGUMENT;
    }
    
    int result = delete_stu(head, id);

    if (result == 1) {
        printf("Student deleted.\n");
        return SHELL_OK;
    } else {
        printf("Error: student not found.\n");
        return SHELL_ERR_STUDENT_NOT_FOUND;
    }
}

ShellResult handle_update(char* args, Student **head) {
    int id; int score;
    int parse = sscanf(args, "%d %d", &id, &score);
    if (parse != 2) {
        printf("Error: invalid argument.\n");
        return SHELL_ERR_INVALID_ARGUMENT;
    }

    if (id <= 0) {
        printf("Error: invalid ID.\n");
        return SHELL_ERR_INVALID_ARGUMENT;
    }

    if (!(0 <= score && score <= 100)) {
        printf("Error: invalid score.\n");
        return SHELL_ERR_INVALID_SCORE;
    }
    
    int result = update_stu(head, id, score);

    if (result == 1) {
        printf("Student updated.\n");
        return SHELL_OK;
    } else {
        printf("Error: student not found.\n");
        return SHELL_ERR_STUDENT_NOT_FOUND;
    }
}

ShellResult handle_find(char* args, Student **head) {
    int id;
    int parse = sscanf(args, "%d", &id);
    if (parse != 1) {
        printf("Error: invalid argument.\n");
        return SHELL_ERR_INVALID_ARGUMENT;
    }

    if (id <= 0) {
        printf("Error: invalid ID.\n");
        return SHELL_ERR_INVALID_ARGUMENT;
    }

    Student *result = find_stu(*head, id);

    if (result == NULL) {
        printf("Error: student not found.\n");
        return SHELL_ERR_STUDENT_NOT_FOUND;
    } else {
        printf("ID: %d\n", result->id);
        printf("Name: %s\n", result->name);
        printf("Score: %d\n", result->score);
        return SHELL_OK;
    }
}

ShellResult handle_list(char* args, Student **head) {
    list_stu(*head);
    return SHELL_OK;
}

ShellResult handle_stats(char* args, Student **head) {
    stats_stu(*head);
    return SHELL_OK;
}

ShellResult handle_help(char* args, Student **head) {
    (void)args;
    (void)head;
    printf("Commands:\n");
    for (int i = 0; i < num_commands; i++) {
        printf("%-30s%s\n", commands[i].usage, commands[i].description);
    }
    return SHELL_OK;
}

ShellResult handle_clear(char* args, Student **head) {
    printf("\033[2J\033[H");
    return SHELL_OK;
}

ShellResult handle_exit(char* args, Student **head) {
    free_stu(*head);
    *head = NULL;
    printf("Goodbye.\n");
    return SHELL_EXIT;
}