#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
    char   *line = NULL;
    size_t  cap  = 0;
    ssize_t nread;

    while (1) {
        write(STDOUT_FILENO, "msh> ", 5);

        nread = getline(&line, &cap, stdin);

        if (nread == -1) {
            write(STDOUT_FILENO, "\n", 1);
            break;
        }

        write(STDOUT_FILENO, line, nread);
    }

    free(line);
    return 0;
}
