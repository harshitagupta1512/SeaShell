#include "def.h"

int getParameters(char *command, int *j, int *s) {

    int i = 4;
    char num[50];
    int itr = 0;

    while (command[i] != '\0' && command[i] != ' ') {

        num[itr] = command[i];
        i++;
        itr++;
    }
    num[itr] = '\0';
    if (command[i] == '\0')
        return 0;

    char *ptr;
    *j = strtol(num, &ptr, 10);
    if (*j == 0) {
        print_red();
        printf("Error: %s: Invalid argument for sig\n", num);
        print_reset();
        return 0;
    }
    i++;
    itr = 0;
    char num2[50];
    while (command[i] != '\0' && command[i] != ' ') {

        num2[itr] = command[i];
        i++;
        itr++;
    }
    num2[itr] = '\0';

    if (command[i] == ' ')
        return 0;

    *s = strtol(num2, &ptr, 10);
    if (strcmp(num2, "0") != 0 && *s == 0) {
        print_red();
        printf("Error: %s: Invalid argument for sig\n", num);
        print_reset();
        return 0;
    }

    return 1;
}

void sig(char *command) {
    // command format is sig job_number signal_number
    int job_number = 0;
    int signal_number = 0;
    int x = getParameters(command, &job_number, &signal_number);

    if (x == 0) {
        print_red();
        printf("Invalid Command\n");
        print_reset();
    }
    int pid = getBgPIDbyJ(job_number);
    if (pid <= 0) {
        print_red();
        perror("Invalid job number\n");
        print_reset();
        return;
    }
    int ret = kill(pid, signal_number);

    if (ret == -1) {
        print_red();
        perror(RED "Error ");
        print_reset();
    }
    // if (signal_number == 9)
    //    deleteEleByPID(pid);
}