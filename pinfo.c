#include "def.h"

void getProcessID(char *command, int *pid) {
    if (strlen(command) == 5) {
        *pid = getpid();
        return;
    }

    if (countSpaces(command) != 1) {
        print_red();
        printf(RED "Invalid number of Arguments\n");
        print_reset();
        return;
    }

    int i = 6;
    char p[strlen(command) - 6];
    int itr = 0;

    while (command[i] != '\0') {
        p[itr] = command[i];
        i++;
        itr++;
    }
    *pid = atoi(p);
}

void pinfo(char *command) {
    int shell_pid = getpid();
    int pid = 0;
    getProcessID(command, &pid);

    char executable[max_size];

    char vm[max_size];

    char buf[max_size] = " ";
    char file[max_size] = " ";

    sprintf(file, "/proc/%d/exe", pid);

    if (readlink(file, buf, sizeof(buf) - 1) < 0) {
        perror(RED "Executable Path not found\n");
    } else {

        strcpy(executable, buf);

        if (strcmp(executable, homeDir) == 0) {
            strcpy(executable, "~");
        } else if (strstr(executable, homeDir) != NULL) {
            unsigned long int x = strlen(homeDir);
            char newCurrDir[max_size] = "~";
            strcat(newCurrDir, executable + x);
            strcpy(executable, newCurrDir);
        }

        sprintf(file, "/proc/%d/stat", pid);
        FILE *fd;
        fd = fopen(file, "r");
        char processStatus[3];
        char line[max_size];
        fgets(line, sizeof(line), fd);

        char *t;
        t = strtok(line, " ");
        int itr = 1;
        while (t != NULL) {
            if (itr == 3)
                strcpy(processStatus, t);

            else if (itr == 23)
                strcpy(vm, t);

            t = strtok(NULL, " ");
            itr++;
        }

        char *ptr;
        unsigned long int virtual_mem = strtol(vm, &ptr, 10);

        fclose(fd);

        if (strcmp(command, "pinfo") == 0 || pid == shell_pid)
            strcat(processStatus, "+");


        print_green();
        printf("PID -- %d \nProcess Status -- %s\nMemory -- %ld kB\nExecutable Path -- %s\n", pid, processStatus,
               virtual_mem / 1024,
               executable);
        print_reset();
    }
}