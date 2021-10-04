#include "def.h"

int createPointers(char *command, char **argumentPointers) {
    unsigned long int l = strlen(command);
    unsigned long int i = 0;
    unsigned long int itr = 0;

    while (command[i] != '\0' && i < l) {
        unsigned long int j = 0;
        char arg[max_size];
        while (command[i] != ' ' && command[i] != '\0') {
            arg[j] = command[i];
            j++;
            i++;
        }

        arg[j] = '\0';
        argumentPointers[itr] = (char *) malloc((j + 2) * sizeof(char));
        strcpy(argumentPointers[itr], arg);
        itr++;

        if (command[i] == '\0')
            break;

        i++;
    }

    return 1;
}

void system_commands(char *command) {

    unsigned long int l = strlen(command);

    if (command[l - 1] == '&') {
        if (command[l - 2] == ' ')
            command[l - 2] = '\0';
        else
            command[l - 1] = '\0';

        background(command);
    } else
        foreground(command);
}
