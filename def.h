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
#define max_size 1000
#define RED "\x1B[31m"
#define GREEN "\x1B[32m"

char lastVisitedDir[max_size];

void prompt(char* home);
void echo(char* command);
void pwd(char* home);
void cd(char* command, char* home);
void ls(char* command, char* home);
int countSpaces(char* str);
void system_commands(char* command);
void foreground(char* command);
void background(char* command);
void pinfo(char* command);
void findParent(char *parent, char* currDir);

void createPointers(char* command, char** argumentPointers);

#endif //OSN_ASSI2_DEF_H
//HANDLE MULTIPLE DIR NAMES IN LS
