#include "def.h"

void prompt(char* home)
{
    //The directory in which the file exists is the home directory
    char username[max_size];
    getlogin_r(username, max_size);

    char systemName[max_size];
    gethostname(systemName, max_size);

    char currDir[max_size];
    getcwd(currDir, max_size);

    if(strcmp(currDir,home)==0)
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

    char finalPrompt[max_size]="<";
    strcat(finalPrompt,username);
    strcat(finalPrompt,"@");
    strcat(finalPrompt,systemName);
    strcat(finalPrompt,":");
    strcat(finalPrompt,currDir);
    strcat(finalPrompt,">");
    printf( "\x1b[34m" "%s" ,finalPrompt);
}
