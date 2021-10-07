#include "def.h"

struct redirec {
    int isInput;
    int isOutputAppend;
    int isOutputOverwrite;
    char inputFile[max_size];
    char outputFile[max_size];
    char task[max_size];
};

int getRedirecStructure(struct redirec *R, char *command) {
    char params[500][500];
    int size = 0;
    for (int i = 0; command[i] != '\0'; ++i) {
        if (command[i] == ' ') {
            continue;
        }
        int j = i;
        for (j = i; command[j] != '\0' && command[j] != ' '; ++j) {
            params[size][j - i] = command[j];
        }
        params[size][j - i] = '\0';
        size++;
        if (command[j] == '\0') {
            break;
        } else {
            i = j;
        }
    }

    if (size == 0) {
        return 0;
    }
    /*  for (int i = 0; i < size; i++)
          printf("--%s--\n", params[i]);
      */
    strcpy(R->task, params[0]);
    int f = 0;

    for (int i = 1; i < size; i++) {
        if (strcmp(params[i], "<") == 0) {
            R->isInput = 1;
            i++;
            if (i >= size)
                return 0;
            strcpy(R->inputFile, params[i]);
            f = 1;
        } else if (strcmp(params[i], ">") == 0) {
            R->isOutputOverwrite = 1;
            R->isOutputAppend = 0;
            i++;
            if (i >= size)
                return 0;
            strcpy(R->outputFile, params[i]);
            f = 2;

        } else if (strcmp(params[i], ">>") == 0) {
            R->isOutputAppend = 1;
            R->isOutputOverwrite = 0;
            i++;
            if (i >= size)
                return 0;
            strcpy(R->outputFile, params[i]);
            f = 3;
        } else {
            if (f == 1)
                strcpy(R->inputFile, params[i]);
            else if (f == 2 || f == 3)
                strcpy(R->outputFile, params[i]);
            else {
                strcat(R->task, " ");
                strcat(R->task, params[i]);
            }
        }
    }

    return 1;
}


int isRedirec(char *command) { //does the input command involve redirection ?
    int l = strlen(command);
    for (int i = 0; i < l; i++)
        if (command[i] == '<' || command[i] == '>')
            return 1;

    return 0;
}

void io_redirection(char *command) {

    printf("Here\n");
    struct redirec R;
    R.isOutputAppend = 0;
    R.isInput = 0;
    R.isOutputOverwrite = 0;
    strcpy(R.task, "");
    strcpy(R.inputFile, "");
    strcpy(R.outputFile, "");
    int x = getRedirecStructure(&R, command);

    if (x == 0) {
        printf(RED "Invalid Command");
        return;
    }
    printf("Command = %s, Input File = %s, Output File = %s\n", R.task, R.inputFile, R.outputFile);
    pid_t pid = fork();
    if (pid < 0) {
        printf(RED
               "Fork Failed\n");
        return;
    }

    if (pid == 0) {
        //child process
        if (R.isInput == 1) {
            int fd_in = open(R.inputFile, O_RDONLY, 0);
            if (fd_in < 0) {
                printf(RED
                       "Input File does not exist\n");
                return;
            }
            dup2(fd_in, 0);
            close(fd_in);
        }
        if (R.isOutputAppend == 1 || R.isOutputOverwrite == 1) {
            int fd_out = 0;

            if (R.isOutputOverwrite == 1)
                fd_out = open(R.outputFile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            else if (R.isOutputAppend == 1)
                fd_out = open(R.outputFile, O_WRONLY | O_CREAT | O_APPEND, 0644);

            if (fd_out < 0) {
                perror("Could not open output file\n");
                return;
            }

            dup2(fd_out, 1);
            close(fd_out);
        }
        runCommand(R.task);

        dup2(saved_stdin, 0);
        //close(saved_stdin);
        dup2(saved_stdout, 1);
        //close(saved_stdout);

    } else {
        //parent process
        int status;
        waitpid(pid, &status, WUNTRACED);
    }
}
