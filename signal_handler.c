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
            kill(child_pid, SIGKILL);
            printf(RED "\n%s with pid : %d stopped after receiving signal\n", bgProc[i].procName, child_pid);
            bgProc[i].stopped = 1;
        } else {
            printf(RED "%s with pid %d exited abnormally\n", bgProc[i].procName, child_pid);
            bgProc[i].terminated = 1;
        }
    }

}

void SIGINT_handler(int signal) {
    pid_t pid = getpid();
    if (pid < 0) {
        perror(RED "ERROR: ");
        return;
    } else {
        write_history();
        exit(1);
    }
}

