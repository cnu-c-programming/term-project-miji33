/*
 * main.c  –  Mini Student Shell
 *
 * TODO: Implement admin_shell and client_shell.
 *
 * Build:
 *   make admin   →  admin_shell  (compiled with -DADMIN_MODE)
 *   make client  →  client_shell (compiled with -DCLIENT_MODE)
 *
 * Usage:
 *   ./admin_shell [students.csv]
 *   ./admin_shell -f commands.txt [students.csv]
 *   ./client_shell [students.csv]
 *   ./client_shell -f commands.txt [students.csv]
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* TODO: Add your own header includes here */
#include "student.h"
#include "file_io.h"
#include "command.h"

/* ---------------------------------------------------------------
 * TODO: Implement the interactive shell loop.
 *   - Print a prompt and read a line from stdin.
 *   - Parse the line into a command and arguments.
 *   - Dispatch to the appropriate handler function.
 *   - Loop until the user types "exit" or EOF.
 * --------------------------------------------------------------- */
void run_shell(const char *csv_path) {
    g_csv_path = csv_path;
    Student *head = NULL;
    int cnt = reload(&head, csv_path);
    
    #ifdef ADMIN_MODE
    printf("[Admin Program]\n");
    #endif
    #ifdef CLIENT_MODE
    printf("[Client Program]\n");
    #endif

    printf("Loaded %d students from %s.\n", cnt, csv_path);
    
    char line[256];
    while (1) {
        #ifdef ADMIN_MODE
        printf("admin> ");
        #endif
        #ifdef CLIENT_MODE
        printf("client> ");
        #endif
        
        if (fgets(line, sizeof(line), stdin) == NULL) break;

        line[strcspn(line, "\n")] = '\0';

        char *cmd = strtok(line, " ");
        if (cmd == NULL) continue;
        char *args = strtok(NULL, "");

        int found = 0;
        ShellResult result = SHELL_OK;
        for (int i = 0; i < num_commands; i++) {
            if (strcmp(commands[i].name, cmd) == 0) {
                result = commands[i].handler(args, &head);
                found = 1;
                break;
            }
        }
        if (!found) printf("Unknown command or permission denied.\n");

        if (result == SHELL_EXIT) { break; }   
    }
}

/* ---------------------------------------------------------------
 * TODO: Implement batch mode – read commands from a file.
 *   - Open cmd_file for reading.
 *   - Execute each line as a command (same logic as run_shell).
 *   - Close the file when done.
 * --------------------------------------------------------------- */
void run_command_file(const char *cmd_file, const char *csv_path) {
    g_csv_path = csv_path;
    Student *head = NULL;
    int cnt = reload(&head, csv_path);
    
    #ifdef ADMIN_MODE
    printf("[Admin Program]\n");
    #endif
    #ifdef CLIENT_MODE
    printf("[Client Program]\n");
    #endif

    printf("Loaded %d students from %s.\n", cnt, csv_path);
    
    FILE *f = fopen(cmd_file, "r");
    if (f == NULL) {
        printf("Error: cannot open command file.\n");
        return;
    }

    char line[256];
    int line_num = 1;
    while (1) {
        if (fgets(line, sizeof(line), f) == NULL) break;
        
        line[strcspn(line, "\n")] = '\0';
        if (line[0] == '\0' || line[0] == '#') continue;
        printf("[command file:%d] %s\n", line_num++, line);
        
        char *cmd = strtok(line, " ");
        if (cmd == NULL) continue;
        char *args = strtok(NULL, "");

        int found = 0;
        ShellResult result = SHELL_OK;
        for (int i = 0; i < num_commands; i++) {
            if (strcmp(commands[i].name, cmd) == 0) {
                result = commands[i].handler(args, &head);
                found = 1;
                break;
            }
        }
        
        if (!found) {
            printf("Unknown command or permission denied.\n");
            printf("Skipped line %d.\n", line_num - 1);
        } else if (result != SHELL_OK && result != SHELL_EXIT) {
            printf("Skipped line %d.\n", line_num - 1);
        }
        
        if (result == SHELL_EXIT) { break; }
    }
    fclose(f);
}

int main(int argc, char *argv[]) {
    const char *csv_path  = "students.csv"; /* default CSV file */
    const char *cmd_file  = NULL;           /* -f <file> argument */

    if (argc < 2) {
    #ifdef ADMIN_MODE
        printf("Usage: ./admin_shell <csv_file> [-f command_file]\n");
    #endif
    #ifdef CLIENT_MODE
        printf("Usage: ./client_shell <csv_file> [-f command_file]\n");
    #endif
        return 1;
    }

    for (int i = 1; i < argc; i++) {
            if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
                cmd_file = argv[++i];
            } else {
                csv_path = argv[i];
            }
        }

#ifdef ADMIN_MODE
    /* Admin shell: supports add, delete, update, save, load, sort, list, find, help, exit */
    if (cmd_file) {
        run_command_file(cmd_file, csv_path);
    } else {
        run_shell(csv_path);
    }

#elif defined(CLIENT_MODE)
    /* Client shell: supports find, list, help, exit  (read-only) */
    if (cmd_file) {
        run_command_file(cmd_file, csv_path);
    } else {
        run_shell(csv_path);
    }

#else
#error "Define either -DADMIN_MODE or -DCLIENT_MODE when compiling."
#endif

    return 0;
}
