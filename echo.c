#include "def.h"
void echo(char* command)
{
    char tempCommand[max_size];
    strcpy(tempCommand, command);

    strcpy(tempCommand, tempCommand + 5);
    printf(GREEN "%s\n",tempCommand);
}