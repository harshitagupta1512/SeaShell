#include "def.h"

void removeSpacesAndTabs(char* str)
{
    //REPLACE TABS BY SPACES
    int i = 0;
    while(str[i] != '\0')
    {
        if (str[i] == '\t')
            str[i] = ' ';

        i++;
    }

    //REMOVE SPACES FROM THE BEGINNING
    unsigned long int l = strlen(str);
    int k = 0;
    while(str[k]==' ')
        k++;

    strcpy(str, str + k);
    str[l - k] = '\0';

    //REMOVE SPACES FROM THE END
    unsigned long int j = strlen(str) - 1;
    while(str[j] == ' ')
        j--;

    str[ j + 1 ] = '\0';

    //REMOVE EXTRA SPACES IN BETWEEN
    char blank[max_size];
    int c = 0, d = 0;
    while (str[c] != '\0')
    {
        if (!(str[c] == ' ' && str[c+1] == ' '))
        {
            blank[d] = str[c];
            d++;
        }
        c++;
    }

    blank[d] = '\0';
    strcpy(str,blank);
}

void runCommand(char* currCommand) {

    if (history->NumEle < 20)
        Inject(history, currCommand);
    else {
        Pop(history);
        Inject(history, currCommand);
    }

    char currCommandName[max_size];
    unsigned long int l = strlen(currCommand);
    unsigned long int i = 0;

    while (currCommand[i] != ' ' && i < l) {
        currCommandName[i] = currCommand[i];
        i++;
    }

    currCommandName[i] = '\0';

    if (strcmp(currCommandName, "exit") == 0)
    {
        write_history();
        exit(0);

    }
    else if (strcmp(currCommandName, "echo") == 0)
        echo(currCommand);
    else if (strcmp(currCommandName, "pwd") == 0) {
        pwd();
    }
    else if (strcmp(currCommandName, "cd") == 0)
        cd(currCommand, homeDir);
    else if(strcmp(currCommandName,"pinfo") == 0)
        pinfo(currCommand);
    else if(strcmp(currCommandName,"ls") == 0)
        ls(currCommand, homeDir);
    else if(strcmp(currCommandName, "repeat") == 0)
        repeat(currCommand);
    else if(strcmp(currCommandName, "history") == 0)
        getHistory(currCommand);
    else
        system_commands(currCommand);
}

int main(void)
{
    signal(SIGINT, SIGINT_handler);
    strcpy(lastAddedCommand ,"");
    numBgProc = 0;
    signal(SIGCHLD, SIGCHLD_handler);
    history = initQueue();
    getcwd(homeDir, max_size);
    load_history();
    strcpy(lastVisitedDir, "");

    //homeDir and lastVisitedDir are global variables

    char* command = NULL;
    //command = (char*)malloc(max_size*sizeof (char));

    size_t x;
    x = max_size;
    char* token;
    const char s[2] = ";";      //deliminator
    char currCommand[max_size];

    do
    {
        prompt(homeDir);

        getline((char **) &command, (size_t *) &x, stdin);
        command[strlen(command) - 1] = '\0';
        //REMOVE LAST \n CHARACTER

        token = strtok(command, ";");


        while( token != NULL)
        {
            strcpy(currCommand,token);
            removeSpacesAndTabs(currCommand);
            runCommand(currCommand);
            token = strtok(NULL, ";");
        }

    }while(1);
}
