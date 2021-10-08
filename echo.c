#include "def.h"

void echo(char *command) {
    char tempCommand[max_size];
    strcpy(tempCommand, command);

    strcpy(tempCommand, tempCommand + 5);
    print_green();
    printf("%s\n", tempCommand);
    print_reset();
}