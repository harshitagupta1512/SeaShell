#include "def.h"

int getJ(char *command) {
    int i = 3;
    int itr = 0;
    char num[50];
    while (command[i] != '\0') {
        num[itr] = command[i];
        itr++;
        i++;
    }
    num[itr] = '\0';
    int j = 0;
    char *ptr;
    j = strtol(num, &ptr, 10);
    if (j == 0) {
        print_red();
        printf("Invalid Job Number\n");
        print_reset();
        return -1;
    }
    return j;
}

void fg_command(char *command) {
    //Bring the running or stopped background job corresponding to job number to the foreground, and changes its state to running
    int j = getJ(command);
    if (j < 0 || j == 0) {
        print_red();
        printf("Invalid Job Number\n");
        print_reset();
        return;
    }
    int pid = -1;
    pid = getBgPIDbyJ(j);
    if (pid <= 0) {
        print_red();
        printf("Invalid Command\n");
        print_reset();
        return;
    }
    //SIG_IGN specifies that the signal should be ignored.
    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    /*A process cannot read from or write to the user’s terminal while it is running as a background job.
    When any process in a background tries to read from the terminal,
    If it does it is sent a SIGTTIN or SIGTTOU signal.*/

    /*
    tcgetpgrp() returns the process group ID of the foreground process group on the terminal
    associated to fd i.e. 0, which must be the controlling terminal of the calling process.
    tcsetpgrp() makes the process group with process group ID p the foreground process group
    on the terminal.
    */

    int cpid = pid;
    int e = tcsetpgrp(STDIN_FILENO, cpid);
    if (e < 0) {
        perror(RED "Command Unsuccessful\n");
        return;
    }
    char pname[max_size];
    getPnameByPID(pid, pname);

    int x = kill(pid, SIGCONT); //SIGCONT signal is sent to a process to make it continue

    if (x < 0) {
        perror("Error");
        return;
    }

    int status = 0;
    waitpid(pid, &status, WUNTRACED); //wait till the child process changes state
    // WUNTRACED allows parent to be returned from waitpid() if child gets stopped or exiting or being killed.

    tcsetpgrp(0, getpgrp());

    /* SIG_DFL informs the kernel that there is no user signal handler for the given signal
    and that the kernel should take default action for it
     */
    signal(SIGTTOU, SIG_DFL);
    signal(SIGTTIN, SIG_DFL);

    /*
     WIFSTOPPED , WIFSIGNALED , WIFEXITED , are macros that take an argument status of type int, as returned by waitpid()
     WIFSTOPPED evaluates to 1 when the child process is stopped, 0 otherwise
     WIFSIGNALED evaluates to 1 when the process was terminated with a signal.
     WIFEXITED evaluates to 1 when the process exited by using an exit(2) call.
     */

    if (WEXITSTATUS(status) == EXIT_SUCCESS && WIFEXITED(status)) {
        deleteEleByPID(pid);
        print_yellow();
        printf("%s with pid %d suspended\n", pname, pid);
        print_reset();
    }
}