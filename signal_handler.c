#include "def.h"

void signal_handler(int signal)
{
    int status;
    pid_t child_pid = waitpid(-1, &status, WNOHANG);

    if (child_pid > 0)
    {
        int i = 0;
        for(i = 0; i < numBgProc ; i++)
        {
            if(bgProc[i].pid == child_pid)
                break;
        }

        if (WEXITSTATUS(status) == EXIT_SUCCESS && WIFEXITED(status))
            printf(YELLOW "%s with pid %d exited normally\n", bgProc[i].procName, child_pid);
        else
            printf(RED "---%s with pid %d exited abnormally\n", bgProc[i].procName, child_pid);
    }
}

void SIGINT_handler(int signal)
{
    pid_t pid = getpid();
    if (pid < 0)
    {
        perror(RED "ERROR: ");
        return;
    }
    else
    {
        write_history();
        exit(1);
    }
}

