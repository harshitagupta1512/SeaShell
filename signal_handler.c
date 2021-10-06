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

void signal_handler_CtrlC(int signal) {
    int pid = getpid();
    if (pid < 0) {
        perror("Error ");
        return;
    } else if (pid != shell_pid) {
        deleteEleByPID(pid);
        exit(1);
    } else {
        write_history();
        printf("\n");
        fflush(stdout);
    }
}

void signal_handler_CtrlZ(int signal) {
    int pid = getpid();
    if (pid < 0) {
        perror(RED "Error ");
        return;
    }

    if (pid != shell_pid) {
        int x = kill(pid, SIGTSTP);
        if (x < 0)
            perror(RED "Error");
    }

}




