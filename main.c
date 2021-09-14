#include "def.h"

void removeSpacesAndTabs(char* str)
{

    //HANDLE TAB SPACES
    //LAST CHARACTER IN COMMAND IS A SPACE/TAB
    int i = 0;
    while(str[i] != '\0')
    {
        //replace tabs by spaces
        if (str[i] == '\t')
            str[i] = ' ';

        i++;
    }

    unsigned long int j = strlen(str) - 1;
    while(str[j] == ' ')
    {
        //remove spaces at the end
        j--;
    }
    str[ j + 1 ] = '\0';


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

    printf("--Command received = %s--\n", str);
}

int main()
{
    int myshellPID = getpid();
    char homeDir[max_size];
    getcwd(homeDir, max_size);

    strcpy(lastVisitedDir, homeDir);

    char* command;
    command = (char*)malloc(max_size*sizeof (char));
    size_t x;
    x = max_size;
    char* token;
    const char s[2] = ";";
    char currCommand[max_size];
    char currCommandName[max_size];

    do
    {
        prompt(homeDir);
        getline((char **) &command, (size_t *) &x, stdin);
        command[strlen(command)-1] = '\0';

        token = strtok(command, s);

        while( token != NULL)
        {
            strcpy(currCommand,token);
            removeSpacesAndTabs(currCommand);
            unsigned long int l = strlen(currCommand);
            int i = 0;

            while(currCommand[i] != ' ' && i < l)
            {
                currCommandName[i] = currCommand[i];
                i++;
            }
            currCommandName[i] = '\0';

            if(strcmp(currCommandName,"exit") == 0)
                exit(0);
            else if (strcmp(currCommandName, "echo") == 0)
                echo(currCommand);
            else if (strcmp(currCommandName, "pwd") == 0)
                pwd(homeDir);
            else if (strcmp(currCommandName, "cd") == 0)
                cd(currCommand, homeDir);
            else if(strcmp(currCommandName,"pinfo")==0)
              pinfo(currCommand);
            else if(strcmp(currCommandName,"ls")==0)
                ls(currCommand, homeDir);

            else
            {
                system_commands(currCommand);
            }

            token = strtok(NULL, s);
        }

    }while(1);
}
