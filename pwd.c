#include "def.h"

void pwd(char* home)
{
    char currDir[max_size];
    getcwd(currDir, max_size);

    if(strcmp(currDir,home) == 0)
    {
        strcpy(currDir,"~");
    }

    else if(strstr(currDir,home) != NULL)
    {
        unsigned long int x = strlen(home);
        char newCurrDir[max_size] = "~";
        strcat(newCurrDir,currDir + x);
        strcpy(currDir,newCurrDir);
    }

    printf("\x1B[32m" "%s\n",currDir);
}