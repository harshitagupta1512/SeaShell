#include "def.h"

void bg_command(char *command) {

    // Changes the state of a stopped background job to running (in the background).
    int j = getJ(command);
    if (j < 0 || j == 0) {
        print_red();
        perror("Invalid Job Number\n");
        print_reset();
        return;
    }

    int pid = -1;
    pid = getBgPIDbyJ(j);

    if (pid <= 0) {
        print_red();
        perror("Invalid Command");
        print_reset();
        return;
    }

    int x = kill(pid, SIGCONT); //SIGCONT signal is sent to a process to make it continue

    if (x < 0) {
        perror("Error");
        return;
    }
}