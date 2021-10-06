#include "def.h"

void sort(struct node temp[totalBgProc], int n) {

    struct node p;
    for (int i = 1; i < n; i++)
        for (int j = 0; j < n - i; j++) {
            if (strcmp(temp[j].name, temp[j + 1].name) > 0) {
                p = temp[j];
                temp[j] = temp[j + 1];
                temp[j + 1] = p;
            }
        }
}

int getFlags(char *command, int *isR, int *isS) {
    if (strlen(command) == 4) {
        *isR = 1;
        *isS = 1;
        return 1;
    } else {
        int i = 5;
        if (command[i] != '-')
            return 0;
        i++;
        if (command[i] == 'r') {
            if (command[i + 1] == 's') {
                *isR = 1;
                *isS = 1;
                i++;
                i++;
            } else {
                *isR = 1;
                *isS = 0;
                i++;
            }
            return 1;
        } else if (command[i] == 's') {
            if (command[i] == 'r') {
                *isS = 1;
                *isR = 1;
            } else {
                *isS = 1;
                *isR = 0;
            }
            return 1;

        } else
            return 0;
    }
}

int getStatus(int pid, char *status) {
    char file[max_size];
    sprintf(file, "/proc/%d/stat", pid);
    FILE *fd;
    fd = fopen(file, "r");
    if (fd <= 0) {
        deleteEleByPID(pid); //process/file not found
        return -1;
    }
    char processStatus[3];
    char line[max_size];
    fgets(line, sizeof(line), fd);

    char *t;
    t = strtok(line, " ");
    int itr = 1;
    while (t != NULL) {
        if (itr == 3)
            strcpy(processStatus, t);
        t = strtok(NULL, " ");
        itr++;
    }

    if (strcmp(processStatus, "T") == 0)
        strcpy(status, "Stopped");
    else if (strcmp(processStatus, "S") == 0 || strcmp(processStatus, "R") == 0)
        strcpy(status, "Running");
    else
        strcpy(status, processStatus);
}

void jobs(char *command) {

    int isR = 0;
    int isS = 0;
    int x = getFlags(command, &isR, &isS);
    if (x == 0) {
        printf(RED "Invalid Command\n");
        return;
    }
    struct node temp[totalBgProc];
    char status[totalBgProc][max_size];

    for (int i = 0; i < totalBgProc; i++) {

        temp[i] = bgProcess[i];
    }

    sort(temp, totalBgProc);

    for (int i = 0; i < totalBgProc; i++) {
        getStatus(temp[i].pid, status[i]);
    }

    for (int i = 0; i < totalBgProc; i++)
        if ((isS == 1 && strcmp(status[i], "Stopped") == 0) || (isR == 1 && strcmp(status[i], "Running") == 0))
            printf(GREEN "[%d] %s %s [%d]\n", temp[i].job_num, status[i], temp[i].name, temp[i].pid);


}