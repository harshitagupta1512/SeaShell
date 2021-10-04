#include "def.h"

int isPipe(char *command) {
    int i = 0;
    while (command[i] != '\0') {
        if (command[i] == '|')
            return 1;
        i++;
    }
    return 0;
}

void piping(char *command) {

    char tempCommand[max_size];
    strcpy(tempCommand, command);
    char arrayCommands[max_size][max_size];
    int itr = 0;
    char s[2];
    s[0] = '|';
    s[1] = '\0';
    char *token;
    token = strtok(tempCommand, s);

    while (token != NULL) {
        strcpy(arrayCommands[itr], token);
        itr++;
        token = strtok(NULL, s);
    }
    int numCommands = itr;

    for (int i = 0; i < numCommands; i++) {
        CleanUpCommand(arrayCommands[i]);
    }

    int saved_stdout = dup(STDOUT_FILENO);
    int saved_stdin = dup(STDIN_FILENO);


    int pipesArray[numCommands][2];
    for (int i = 0; i < numCommands; i++) {
        int x = pipe(pipesArray[i]);
        if (x == -1) {
            perror(RED "Piping unsuccessful \n");
            return;
        }
    }

    //printf(YELLOW"%d\n", numCommands);
    for (int i = 0; i < numCommands - 1; i++) {

        //printf(YELLOW"%s\n", arrayCommands[i]);

        if (i == 0) {
            dup2(pipesArray[i][1], 1);
            close(pipesArray[i][1]);
            runCommand(arrayCommands[i]);
            dup2(saved_stdout, 1);
            //close(saved_stdout);
        } else {
            dup2(pipesArray[i][1], 1);
            dup2(pipesArray[i - 1][0], 0);
            close(pipesArray[i][1]);
            close(pipesArray[i - 1][0]);
            runCommand(arrayCommands[i]);
            dup2(saved_stdin, 0);
            //close(saved_stdin);
            dup2(saved_stdout, 1);
            //close(saved_stdout);
        }
    }

    //For last command
    dup2(pipesArray[numCommands - 2][0], 0);
    close(pipesArray[numCommands - 2][0]);
    runCommand(arrayCommands[numCommands - 1]);
    dup2(saved_stdin, 0);
    close(saved_stdin);
    close(saved_stdout);

}