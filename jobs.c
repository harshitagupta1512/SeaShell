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
    char params[500][500];
    int size = 0;
    for (int i = 0; command[i] != '\0'; ++i) {
        if (command[i] == ' ') {
            continue;
        }
        int j = i;
        for (j = i; command[j] != '\0' && command[j] != ' '; ++j) {
            params[size][j - i] = command[j];
        }
        params[size][j - i] = '\0';
        size++;
        if (command[j] == '\0') {
            break;
        } else {
            i = j;
        }
    }

    if (size == 0) {
        return 0;
    }
    if (strcmp(params[0], "jobs") != 0)
        return 0;

    if (strcmp(command, "jobs") == 0) {
        *isR = 1;
        *isS = 1;
        return 1;
    }

    for (int i = 1; i < size; i++) {
        if (strcmp(params[i], "-r") == 0)
            *isR = 1;
        else if (strcmp(params[i], "-rs") == 0 || strcmp(params[i], "-sr") == 0) {
            *isR = 1;
            *isS = 1;
            return 1;
        } else if (strcmp(params[i], "-s") == 0)
            *isS = 1;
        else
            return 0;
    }
    return 1;
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