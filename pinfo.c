#include "def.h"
void getProcessID(char* command, int pid)
{
    if(strlen(command) == 5)
    {   pid = getpid();
        return;
    }

    if(countSpaces(command) != 1 ) {
        printf(RED "Invalid number of Arguments\n");
        return;
    }
    int i = 6;
    char p[strlen(command) - 6];
    int itr = 0;

    while(command[i] != '\0')
    {
        p[i]
    }





}

void pinfo(char* command)
{
    int pid = 0;
    getProcessID(command, pid);

    char executable[max_size];
    char processStatus;
    int vm;

    char buf[max_size] = " ";
    char file[max_size] = " ";
    sprintf(file, "/proc/%d/exe", pid);

    if (readlink(file, buf, sizeof(buf) - 1) < 0)
    {
        perror("Executable Path not found\n");
    }
    strcpy(executable, buf);

    //cat /proc/type_pid_here/stat
    sprintf(file, "/proc/%d/stat", pid);
    FILE* fd;
    fd = fopen(file,r);
    fscanf(fd, "%*d %*s %c %*d %*d %*d %*d %*d %*u %*lu %*lu %*lu %*lu %*lu %*lu %*lu %*ld %*ld %*ld %*llu %lu",processStatus,&vm);
    fclose(fd);
    printf("pid = %d \nProcess Status = %c\nMemory = %d\nExecutable Path = %s",pid, processStatus, vm, executable);

}