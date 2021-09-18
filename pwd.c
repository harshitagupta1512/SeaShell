#include "def.h"

void pwd()
{
    char currDir[max_size];
    getcwd(currDir, max_size);
    printf(GREEN "%s\n",currDir);
}