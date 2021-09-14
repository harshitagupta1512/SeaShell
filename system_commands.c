#include "def.h"

void createPointers(char* command, char** argumentPointers)
{
    unsigned long int l = strlen(command);
    int i = 0;
    int itr = 0;

    while( command[i] != '\0' && i < l)
    {
        int j = 0;
        char arg[max_size];
        while (command[i] != ' ')
        {
            arg[j] = command[i];
            j++;
            i++;
        }

        arg[j] = '\0';
        printf("%s\n",arg);

        argumentPointers[itr] = (char*) malloc(j * sizeof (char));
        strcpy(argumentPointers[itr],arg);
        itr++;
        i++;
    }
}
void foreground(char* command) {

    int pid = fork();
    if (pid < 0)
        printf("\x1B[31m" "Fork Failed\n");
    else if (pid > 0) {
        //parent process
        int status;
        waitpid(pid, &status, WUNTRACED);
    } else {
        //child process
        int x = countSpaces(command);

        char *argumentPointers[x + 2];
        createPointers(command, argumentPointers);

        argumentPointers[x+1] = NULL;

        int st = execvp(argumentPointers[0], argumentPointers);

        if (st < 0)
            printf("\x1B[31m" "Error - Execution Failed\n");

        exit(0);
    }
}

void background(char* command)
{
    int pid = fork();

    if (pid < 0)
        printf("\x1B[31m" "Fork Failed\n");

    else if (pid > 0)
    {
        //parent process
    }
    else
    {
        //child process
        int x = countSpaces(command);

        char *argumentPointers[x + 2];
        createPointers(command, argumentPointers);

        argumentPointers[x+1] = NULL;

        int p = getpid();
        printf("Background Process PID = %d, System Command = %s\n",p,argumentPointers[0]);

        int st = execvp(argumentPointers[0], argumentPointers);

        if (st < 0)
            printf("\x1B[31m" "Error - Execution Failed\n");


        exit(0);
    }
}

void system_commands(char* command)
{
    unsigned long int l = strlen(command);
    if( command[ l - 1 ] == '&' )
    {command[l-1] = '\0';
        background(command);}
    else
        foreground(command);
}
