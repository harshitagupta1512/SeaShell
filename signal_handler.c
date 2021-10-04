#include "def.h"

void SIGCHLD_handler(int signal) {
    int status;
    pid_t child_pid = waitpid(-1, &status, WNOHANG | WUNTRACED);

    if (child_pid > 0) {
        int i;
        int flag = 0;
        for (i = 0; i < numBgProc; i++) {
            if (bgProc[i].pid == child_pid && bgProc[i].terminated == 0 && bgProc[i].stopped == 0) {
                flag = 1;
                break;
            }
        }
        if (flag == 0)
            return;

        if (WEXITSTATUS(status) == EXIT_SUCCESS && WIFEXITED(status)) {
            printf(YELLOW "%s with pid %d exited normally\n", bgProc[i].procName, child_pid);
            bgProc[i].terminated = 1;
        } else if (WIFSTOPPED(status) || WIFSIGNALED(status)) {
            //kill(child_pid, SIGKILL);
            printf(RED "%s with pid : %d stopped after receiving signal\n", bgProc[i].procName, child_pid);
            bgProc[i].stopped = 1;
        } else {
            printf(RED "%s with pid %d exited abnormally\n", bgProc[i].procName, child_pid);
            bgProc[i].terminated = 1;
        }
    }

}

/*
CtrlC tells the terminal to send a SIGINT to the current foreground process,
which by default translates into terminating the application.
CtrlD tells the terminal that it should register an EOF on standard input,
which bash interprets as a desire to exit.
*/

void signal_handler_CtrlC(int signal) {
    int pid = getpid();
    printf("PID = %d\n", pid);
    if (pid == 0)
        runCommand("exit");
    else {
        printf("\n");
        fflush(stdout);
    }
}

void signal_handler_CtrlZ(int signal) {
    printf("\n");
    fflush(stdout);
}




