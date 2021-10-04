#include "def.h"

void bg_command(char *command) {

    // Changes the state of a stopped background job to running (in the background).
    int j = getJ(command);
    if (j < 0 || j == 0 || j > numBgProc) {
        perror(RED "Invalid Job Number\n");
        return;
    }
    int pid = -1;
    pid = bgProc[j - 1].pid;
    if (pid <= 0) {
        perror(RED "Invalid Command");
        return;
    }

    int x = kill(pid, SIGCONT); //SIGCONT signal is sent to a process to make it continue

    if (x < 0) {
        perror("Error");
        return;
    }
}