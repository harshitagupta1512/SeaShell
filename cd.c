#include "def.h"
int countSpaces(char* str)
{
    int i = 0;
    int count = 0;
    while(str[i]!='\0')
    {
        if(str[i]==' ')
            count++;
        i++;
    }

    return count;
}

void findParent(char *parent, char* currDir)
{
    unsigned long int l = strlen(currDir);
    unsigned long int i;
    for(i = l - 1 ; i >= 0 ; i--)
    {
        if(currDir[i] == '/')
            break;
    }

    unsigned long int itr = 0;

    for(unsigned long int j = 0 ; j < i ; j++)
    {
        parent[itr] = currDir[j];
        itr++;
    }

    parent[itr] = '\0';
}


void cd(char* command, char* home) {

    char tempLastDir[max_size];
    strcpy(tempLastDir, lastVisitedDir);

    char currDir[max_size];
    getcwd(currDir, max_size);
    strcpy(lastVisitedDir, currDir);

    int x = countSpaces(command);

    if (x > 1) {
        printf(RED "Error - more than 1 command line arguments are not allowed for cd\n" );
        return;
    }

    if (strcmp(command,"cd") == 0)
    {   chdir(home);
        return;
    }

    if(strcmp(command,"cd ~") == 0)
    {   chdir(home);
        return;
    }

    if(strcmp(command, "cd .") == 0)
        return;

    if (strcmp(command,"cd ..") == 0) //go to the parent directory of the current directory
    {
        /*
        char parent[max_size];
        findParent(parent, currDir);
        if(strcmp( parent ,"") == 0)
            printf("\x1B[31m" "Already at the root Directory\n" );
        int d = chdir(parent);
        if( d < 0 )
            printf( "\x1B[31m" "Error encountered");
        return;
        */
        chdir("..");
        return;
    }

    if(strcmp(command,"cd -") == 0)
    {
        //go to the last visited directory
        if(strcmp(tempLastDir, "") == 0)
        {
            printf(RED "Error: cd: OLDPWD not set\n");
            return;
        }
        else
        {
            chdir(tempLastDir);
            printf(GREEN "%s\n", tempLastDir);
            return;
        }

    }

    if(x == 1)
    {
        //go to the specified directory
        char dirName[max_size];
        strcpy(dirName, command + 3);
        int d = chdir(dirName);
        if( d < 0)
            printf( RED "Error Encountered - No such directory\n");
    }
}
