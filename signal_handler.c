#include "def.h"

void SIGCHLD_handler(int signal) {
    int status;
    pid_t child_pid = waitpid(-1, &status, WNOHANG | WUNTRACED);

    if (child_pid > 0) {
        char pname[max_size];
        int x = getPnameByPID(child_pid, pname);

        if (WEXITSTATUS(status) == EXIT_SUCCESS && WIFEXITED(status)) {
            print_yellow();
            printf("%s with pid %d exited normally\n", pname, child_pid);
            print_reset();
            deleteEleByPID(child_pid);

        } else if (WIFSTOPPED(status)) {
            print_red();
            printf("%s with pid : %d stopped after receiving signal\n", pname, child_pid);
            print_reset();
        } else if (WIFSIGNALED(status)) {
            print_red();
            printf("%s with pid : %d exited after receiving signal\n", pname, child_pid);
            print_reset();
            deleteEleByPID(child_pid);
        } else {
            print_red();
            printf("%s with pid %d exited abnormally\n", pname, child_pid);
            print_reset();
            deleteEleByPID(child_pid);
        }
    }
    //prompt(homeDir);
    //fflush(stdout);
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
    prompt(homeDir);
    prompt(homeDir);
    fflush(stdout);
}

void SIGTSTP_handler(int signal) {
    //CTRL-Z - should push any currently running foreground job into the background, change its state from running to stopped
    int pid = getpid();
    if (pid != shell_pid) {
        // there is a foreground process running
        int e = kill(pid, SIGTSTP);
        if (e < 0)
            perror(RED "Error");
    }

    printf("\n");
    fflush(stdout);
}




