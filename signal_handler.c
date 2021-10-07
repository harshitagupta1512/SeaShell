#include "def.h"

void SIGCHLD_handler(int signal) {
    int status;
    pid_t child_pid = waitpid(-1, &status, WNOHANG | WUNTRACED);

    if (child_pid > 0) {
        char pname[max_size];
        int x = getPnameByPID(child_pid, pname);

        if (WEXITSTATUS(status) == EXIT_SUCCESS && WIFEXITED(status)) {
            printf(YELLOW "%s with pid %d exited normally\n", pname, child_pid);
            deleteEleByPID(child_pid);

        } else if (WIFSTOPPED(status) || WIFSIGNALED(status)) {
            if (x != -1)
                printf(RED "%s with pid : %d stopped after receiving signal\n", pname, child_pid);
        } else {
            printf(RED "%s with pid %d exited abnormally\n", pname, child_pid);
            deleteEleByPID(child_pid);
        }
    }
}

/*
CtrlC tells the terminal to send a SIGINT to the current foreground process,
which by default translates into terminating the application.
CtrlD tells the terminal that it should register an EOF on standard input,
which bash interprets as a desire to exit.
*/

void SIGINT_handler(int signal) {
    //For ctrl C
    printf("\n");
    fflush(stdout);
}

void SIGTSTP_handler(int signal) {
    //for ctrl-Z
    printf("\n");
    fflush(stdout);
}




