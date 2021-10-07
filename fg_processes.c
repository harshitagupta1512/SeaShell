#include "def.h"

void foreground(char *command) {
    char **argument_pointers;
    int x = countSpaces(command);
    argument_pointers = (char **) malloc(sizeof(char *) * (x + 2));
    int y = createPointers(command, argument_pointers);
    argument_pointers[x + 1] = NULL;
    pid_t pid = 0;
    pid = fork();
    if (pid < 0)
        printf(RED "Fork Failed\n");
    else if (pid > 0) {
        //parent process
        dup2(saved_stdout, 1);
        dup2(saved_stdin, 0);

        signal(SIGTTIN, SIG_IGN);
        signal(SIGTTOU, SIG_IGN);
        setpgid(pid, 0);
        tcsetpgrp(STDIN_FILENO, pid);
        int status;
        waitpid(pid, &status, WUNTRACED);
        pid_t pgid_parent = getpgrp();
        tcsetpgrp(STDIN_FILENO, pgid_parent);
        signal(SIGTTIN, SIG_DFL);
        signal(SIGTTOU, SIG_DFL);

    } else {
        //child process
        setpgid(0, 0);
        int st = execvp(argument_pointers[0], argument_pointers);
        if (st < 0)
            perror(RED "Execution Failed");
        exit(0);
    }

    for (int i = 0; i < x + 2; ++i) {
        free(argument_pointers[i]);
    }

    free(argument_pointers);
}