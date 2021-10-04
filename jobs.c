#include "def.h"

void sort(struct proc temp[numBgProc], int n) {

    struct proc p;
    for (int i = 1; i < n; i++)
        for (int j = 0; j < n - i; j++) {
            if (strcmp(temp[j].procName, temp[j + 1].procName) > 0) {
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
            *isR = 1;
            *isS = 0;
            return 1;
        } else if (command[i] == 's') {
            *isS = 1;
            *isR = 0;
            return 1;

        } else
            return 0;
    }
}


void jobs(char *command) {

    int isR = 0;
    int isS = 0;
    int x = getFlags(command, &isR, &isS);
    if (x == 0) {
        printf(RED "Invalid Command\n");
        return;
    }
    struct proc temp[numBgProc];
    char status[numBgProc][max_size];

    for (int i = 0; i < numBgProc; i++) {

        temp[i] = bgProc[i];
    }

    sort(temp, numBgProc);

    for (int i = 0; i < numBgProc; i++) {

        if (temp[i].stopped == 0 && temp[i].terminated == 0)
            strcpy(status[i], "Running");
        else if (temp[i].stopped == 1)
            strcpy(status[i], "Stopped");
        else
            strcpy(status[i], "Terminated");

    }
    
    for (int i = 0; i < numBgProc; i++) {
        if ((isR == 1 && strcmp(status[i], "Running") == 0) ||
            (isS == 1 && strcmp(status[i], "Stopped") == 0))
            printf(GREEN "[%d] %s %s [%d]\n", temp[i].bgJobNumber, status[i], temp[i].procName, temp[i].pid);
    }

}