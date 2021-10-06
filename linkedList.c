#include "def.h"
//----array-based implementation of linked list----//

void insertEle(char *name, int pid) {
    strcpy(bgProcess[totalBgProc].name, name);
    bgProcess[totalBgProc].pid = pid;
    numJobs++;
    bgProcess[totalBgProc].job_num = numJobs;
    totalBgProc++;
}

int getBgPIDbyJ(int j) {

    for (int i = 0; i < totalBgProc; i++)
        if (bgProcess[i].job_num == j)
            return bgProcess[i].pid;

    return -1;
}

int getPnameByPID(int pid, char *name) {
    for (int i = 0; i < totalBgProc; i++)
        if (bgProcess[i].pid == pid) {
            strcpy(name, bgProcess[i].name);
            return 1;
        }

    return -1;
}

void deleteEleByPID(int pid) {

    int i = 0;
    for (i = 0; i < totalBgProc; i++)
        if (bgProcess[i].pid == pid)
            break;

    if (i < totalBgProc) {
        totalBgProc = totalBgProc - 1;
        for (int j = i; j < totalBgProc; j++)
            bgProcess[j] = bgProcess[j + 1];
    }
}