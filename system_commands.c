#include "def.h"

void createPointers(char* command, char** argumentPointers)
{
    unsigned long int l = strlen(command);
    unsigned long int i = 0;
    unsigned long int itr = 0;

    while( command[i] != '\0' && i < l)
    {
        unsigned long int j = 0;
        char arg[max_size];
        while (command[i] != ' ')
        {
            arg[j] = command[i];
            j++;
            i++;
        }

        arg[j] = '\0';

        argumentPointers[itr] = (char*) malloc(j * sizeof (char));
        strcpy(argumentPointers[itr],arg);
        itr++;
        i++;
    }
}

void system_commands(char* command)
{

    unsigned long int l = strlen(command);

    if( command[ l - 1 ] == '&' )
    {
        if(command[ l - 2 ] == ' ')
            command[l-2] = '\0';
        else
            command[l-1] = '\0';

        background(command);
    }
    else
        foreground(command);
}
