# msh 
Title: Minimal Shell
Name: Mike Owen Otieno
Student ID: SCT212-0078/2024

## building and running
'''bash
make
./msh

Features Checklist:
 Task 1: Prompt, getline, and EOF clean exit (Complete)
 Task 2: Tokenization and ignoring whitespace (Complete)
 Task 3: fork/execvp/waitpid for external commands (Complete)
 Task 4: Built-ins (cd, help, exit) (Complete)
 Task 5: Exit status in prompt (Complete)
 Task 6: Background execution with & (Complete)
 Task 7: Output redirection > (Complete)
 Task 8: SIGINT handling (Ctrl-C) (Complete)

Design explanation
The shell uses a while loop to read commands. First, it prints the prompt using write(). Then it reads the input using getline(), which handles lines of any length and memory allocation automatically. If getline() returns -1, it means the user pressed Ctrl-D, so the loop breaks, and we free the line buffer before exiting.

After reading, strtok() splits the line into an array of arguments (argv). If the command is cd, help, or exit, the shell handles it internally. I made cd a built-in because if it were an external program, it would run in a child process. The child would change its own directory, but the parent shell's directory would remain the same. The same applies to exit.

For external commands, the shell calls fork(). In the child process, we check for redirection (>). If found, we open the file and use dup2 to redirect stdout. Then we call execvp(). If execvp fails (e.g., command not found), it prints an error and exits with 127.

In the parent process, if the command is not a background job (&), we use waitpid() to wait for the child to finish. We use WIFEXITED and WIFSIGNALED to get the status. For background jobs, the parent doesn't wait, but at the end of the loop, we call waitpid(-1, NULL, WNOHANG) repeatedly to reap any finished background jobs and prevent zombie processes.

Known limitations
No pipes (|) supported, only output redirection (>).

No job control (no fg or bg).

Environment variables (like $HOME) are not expanded in commands.

 Sources & AI Declaration:
I used online platforms like GeeksforGeeks and Linux man pages to understand the fork/exec/wait cycle, signal handling, and file descriptors. I also used AI for troubleshooting compiler errors and better explaining the logic in the design explanation.
