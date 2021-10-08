#include "def.h"

void repeat(char *command) {
    unsigned long int i = 7;
    char number[max_size];
    int itr = 0;
    while (command[i] != ' ') {
        number[itr] = command[i];
        i++;
        itr++;
    }
    number[itr] = '\0';
    char *ptr;
    unsigned long int num = strtol(number, &ptr, 10);
    if (num == 0) {
        print_red();
        printf("Error: %s: Invalid argument for repeat\n", number);
        print_reset();
        return;
    }
    char temp[max_size];
    strcpy(temp, command);

    strcpy(temp, temp + 7 + itr + 1);

    for (int j = 1; j <= num; j++)
        runCommand(temp);
}

