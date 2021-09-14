#include "def.h"
void echo(char* command)
{
    strcpy(command, command+5);
    printf("\x1B[32m" "%s\n",command);
}