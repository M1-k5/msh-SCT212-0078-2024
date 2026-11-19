#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>

#define MAX_ARGS 64

void print_prompt(int status) {
    char buf[32];
    int len = snprintf(buf, sizeof(buf), "msh[%d]> ", status);
    write(STDOUT_FILENO, buf, len);
}

int main(void) {
    char   *line = NULL;
    size_t  cap  = 0;
    ssize_t nread;
    char   *argv[MAX_ARGS];
    int     last_status = 0;

    signal(SIGINT, SIG_IGN);

    while (1) {
        print_prompt(last_status);

        nread = getline(&line, &cap, stdin);

        if (nread == -1) {
            write(STDOUT_FILENO, "\n", 1);
            break;
        }

        int argc = 0;
        char *token = strtok(line, " \t\n\r");
        while (token != NULL && argc < MAX_ARGS - 1) {
            argv[argc++] = token;
            token = strtok(NULL, " \t\n\r");
        }
        argv[argc] = NULL;

        if (argc == 0) {
            continue;
        }

        if (strcmp(argv[0], "exit") == 0) {
            break;
        }

        if (strcmp(argv[0], "cd") == 0) {
            const char *dir = (argc > 1) ? argv[1] : getenv("HOME");
            if (dir == NULL) {
                write(STDERR_FILENO, "cd: HOME not set\n", 17);
                last_status = 1;
            } else if (chdir(dir) != 0) {
                perror("cd");
                last_status = 1;
            } else {
                last_status = 0;
            }
            continue;
        }

        if (strcmp(argv[0], "help") == 0) {
            const char *help_msg = "msh: simple shell\nBuilt-ins: cd [dir], help, exit\n";
            write(STDOUT_FILENO, help_msg, strlen(help_msg));
            last_status = 0;
            continue;
        }

        int background = 0;
        if (argc > 0 && strcmp(argv[argc - 1], "&") == 0) {
            background = 1;
            argv[argc - 1] = NULL;
            argc--;
        }

        char *redirect_file = NULL;
        for (int i = 0; i < argc; i++) {
            if (strcmp(argv[i], ">") == 0) {
                if (i + 1 < argc) {
                    redirect_file = argv[i + 1];
                    argv[i] = NULL;
                    argc = i;
                }
                break;
            }
        }

        if (argc == 0) {
            continue;
        }

        pid_t pid = fork();

        if (pid < 0) {
            perror("fork failed");
            last_status = 1;
        } else if (pid == 0) {
            signal(SIGINT, SIG_DFL);

            if (redirect_file != NULL) {
                int fd = open(redirect_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd < 0) {
                    perror("open failed");
                    _exit(1);
                }
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }

            execvp(argv[0], argv);
            perror("execvp failed");
            _exit(127);
        } else {
            if (background) {
                char buf[64];
                int len = snprintf(buf, sizeof(buf), "[%d] %d\n", 1, pid);
                write(STDOUT_FILENO, buf, len);
            } else {
                int status;
                waitpid(pid, &status, 0);
                if (WIFEXITED(status)) {
                    last_status = WEXITSTATUS(status);
                } else if (WIFSIGNALED(status)) {
                    last_status = 128 + WTERMSIG(status);
                }
            }
        }

        while (waitpid(-1, NULL, WNOHANG) > 0) {
        }
    }

    free(line);
    return 0;
}
