#include "def.h"
void background(char* command)
{
    int pid = fork();
    if (pid < 0)
        printf("\x1B[31m" "Fork Failed\n");

    else if (pid > 0)
    {
        //parent process
        bgProc[numBgProc].pid = pid;
        int x = countSpaces(command);
        char *argumentPointers[x + 2];
        createPointers(command, argumentPointers);
        argumentPointers[x+1] = NULL;
        strcpy(bgProc[numBgProc].procName, argumentPointers[0]);
        numBgProc++;

        pid_t pgid_parent = getpgrp();
        tcsetpgrp(STDIN_FILENO, pgid_parent);
    }
    else
    {
        //child process
        setpgid(0, 0);

        int x = countSpaces(command);
        char *argumentPointers[x + 2];
        createPointers(command, argumentPointers);
        argumentPointers[x+1] = NULL;

        int p = getpid();

        printf(YELLOW "Background Process PID = %d, System Command = %s\n",p,argumentPointers[0]);

        /*Upon success, exec() never returns to the caller.
        It replaces the current process image, so it cannot return anything to the program
        that made the call. If it does return, it means the call failed.
        */

        int st = execvp(argumentPointers[0], argumentPointers);

        if(st < 0) {
            printf(RED "Execution Failed\n");
            exit(EXIT_FAILURE);
        }
    }
}