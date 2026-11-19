#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_ARGS 64

int main(void) {
    char   *line = NULL;
    size_t  cap  = 0;
    ssize_t nread;
    char   *argv[MAX_ARGS];

    while (1) {
        write(STDOUT_FILENO, "msh> ", 5);

        nread = getline(&line, &cap, stdin);

        if (nread == -1) {
            write(STDOUT_FILENO, "\n", 1);
            break;
        }

        int argc = 0;
        char *token = strtok(line, " \t\n");
        while (token != NULL && argc < MAX_ARGS - 1) {
            argv[argc++] = token;
            token = strtok(NULL, " \t\n");
        }
        argv[argc] = NULL;

        if (argc == 0) {
            continue;
        }

        for (int i = 0; i < argc; i++) {
            write(STDOUT_FILENO, argv[i], strlen(argv[i]));
            write(STDOUT_FILENO, "\n", 1);
        }
    }

    free(line);
    return 0;
}
