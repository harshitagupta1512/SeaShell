#ifndef OSN_ASSI2_DEF_H
#define OSN_ASSI2_DEF_H

#include<stdio.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>
#include <sys/wait.h>
#include<stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <signal.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define max_size 500
#define RED "\x1B[31m"
#define GREEN "\x1B[32m"
#define YELLOW "\x1b[33m"
#define NORMAL  "\x1B[0m"
#define CYAN "\x1b[36m"

//Global Variables
char lastVisitedDir[max_size];
char homeDir[max_size];
char lastAddedCommand[max_size];
int shell_pid;
int saved_stdout;
int saved_stdin;
int isRe;
int isPi;

void print_green();

void print_yellow();

void print_reset();

void print_red();

void print_cyan();

void prompt(char *home);

void echo(char *command);

void pwd();

void cd(char *command, char *home);

void ls(char *command, char *home);

int countSpaces(char *str);

void system_commands(char *command);

void foreground(char *command);

void background(char *command);

void pinfo(char *command);

void findParent(char *parent, char *currDir);

void repeat(char *command);

int createPointers(char *command, char **argumentPointers);

void runCommand(char *currCommand);

int isDirectory(const char *path);

int processCommand(char *command, char dirName[max_size][max_size], int *dir_size, int *isA, int *isL, char *currDir,
                   char *home);

void getPermissions(struct stat buf, char perm[11]);

void getProcessID(char *command, int *pid);

void getHistory(char *command);

void SIGINT_handler(int signal);

void SIGCHLD_handler(int signal);

void CleanUpCommand(char *str);

int isRedirec(char *command);

void io_redirection(char *command);

void jobs(char *command);

int isPipe(char *command);

void piping(char *command);

void sig(char *command);

void fg_command(char *command);

void bg_command(char *command);

void SIGTSTP_handler(int signal);

int getJ(char *command);
//----------------QUEUE------------------//

struct QueueEle {
    char data[max_size];
    struct QueueEle *next;
    struct QueueEle *prev;
};
typedef struct QueueEle *PtrQueueEle;
typedef struct QueueEle QueueEle;
struct Queue {
    PtrQueueEle Front;
    PtrQueueEle Rear;
    int NumEle;
};
typedef struct Queue *Queue;

void Pop(Queue D);                   //Remove from Front
void Inject(Queue D, char *x); //Add at Rear
Queue initQueue();

int IsEmpty(Queue D);

void deleteQueue(Queue D);

void printQueue(Queue D, int number);

void load_history();

void write_history();

//----------------------------------------//
Queue history;


int totalBgProc;
int numJobs;
//-------------linked list (array-based implementation)----------------//

struct node {
    int pid;
    int job_num;
    char name[max_size];
    char status[max_size];
};
struct node bgProcess[max_size]; //Array

void insertEle(char *name, int pid);

int getBgPIDbyJ(int j);

int getPnameByPID(int pid, char *name);

void deleteEleByPID();

//----------------------------------------//
#endif //OSN_ASSI2_DEF_H

// Fix job numbers
// Implement replay

