#include "def.h"

void foreground(char* command) {

    int pid = fork();
    if (pid < 0)
        printf(RED "Fork Failed\n");

    else if (pid > 0) {
        //parent process
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

    }
    else {
        setpgid(0, 0);
        //child process
        int x = countSpaces(command);
        char *argumentPointers[x + 2];
        createPointers(command, argumentPointers);
        argumentPointers[x+1] = NULL;

        int st = execvp(argumentPointers[0], argumentPointers);
        if (st < 0)
            perror(RED "Execution Failed");

        exit(0);
    }
}