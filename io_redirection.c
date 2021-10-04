#include "def.h"

int isRedirec(char *command) { //does the input command involve redirection ?
    int l = strlen(command);
    for (int i = 0; i < l; i++)
        if (command[i] == '<' || command[i] == '>')
            return 1;

    return 0;
}

struct redirec {
    int isInput;
    int isOutputAppend;
    int isOutputOverwrite;
    char inputFile[max_size];
    char outputFile[max_size];
    char task[max_size];
};

int getRedirecStructure(struct redirec *R, char *command) {

    R->isOutputAppend = 0;
    R->isOutputOverwrite = 0;
    R->isInput = 0;
    int flagInput = 0;
    int flagOutput = 0;
    int j = 0;

    while (command[j] != '\0') {
        if (command[j] == '<') {
            R->isInput = 1;
            flagInput++;
        } else if (command[j] == '>') {
            flagOutput++;
            if (command[j + 1] == '>') {
                j++;
                R->isOutputAppend = 1;
            } else
                R->isOutputOverwrite = 1;
        }
        j++;
    }

    if (flagInput >= 2 || flagOutput >= 2) {
        printf(RED
               "Invalid Command\n");
        return 0;
    }

    char word[max_size];
    int itr = 0;
    int i = 0;
    while (command[i] != '>' && command[i] != '<') {
        word[itr] = command[i];
        itr++;
        i++;
    }
    if (word[itr - 1] == ' ')
        word[itr - 1] = '\0';
    else
        word[itr] = '\0';
    strcpy(R->task, word);
    if (command[i] == '<') {

        //task inputFile outputFile
        i++;
        if (command[i] == ' ')
            i++;

        char temp[max_size];
        int pos = 0;

        while (command[i] != '>' && command[i] != '\0') {
            temp[pos] = command[i];
            pos++;
            i++;
        }

        if (temp[pos - 1] == ' ')
            temp[pos - 1] = '\0';
        else
            temp[pos] = '\0';

        strcpy(R->inputFile, temp);

        if (command[i] == '\0')
            return 1;
        else {
            //Get the output file name
            //command[i]  = '>'
            i++;
            if (command[i] == '>')
                i++;

            if (command[i] == ' ')
                i++;

            pos = 0;
            while (command[i] != '\0') {
                temp[pos] = command[i];
                pos++;
                i++;
            }

            temp[pos] = '\0';
            strcpy(R->outputFile, temp);
        }

    } else {
        // task outputFile InputFile

        i++;
        if (command[i] == '>')
            i++;
        if (command[i] == ' ')
            i++;

        char temp[max_size];
        int pos = 0;
        while (command[i] != '<' && command[i] != '\0') {
            temp[pos] = command[i];
            pos++;
            i++;
        }

        if (temp[pos - 1] == ' ')
            temp[pos - 1] = '\0';
        else
            temp[pos] = '\0';

        strcpy(R->outputFile, temp);

        if (command[i] == '\0')
            return 1;
        else {
            //Get the input file name
            //command[i]  = '<'
            i++;
            if (command[i] == ' ')
                i++;

            pos = 0;
            while (command[i] != '\0') {
                temp[pos] = command[i];
                pos++;
                i++;
            }

            temp[pos] = '\0';
            strcpy(R->inputFile, temp);
        }
    }
    return 1;
}

void io_redirection(char *command) {

    struct redirec R;
    int x = getRedirecStructure(&R, command);
    if (x == 0)
        return;

    int saved_stdout = dup(STDOUT_FILENO);
    int saved_stdin = dup(STDIN_FILENO);

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

            runCommand(R.task);

            dup2(saved_stdin, 0);
            close(saved_stdin);
            dup2(saved_stdout, 1);
            close(saved_stdout);
        } else {
            //parent process
            int status;
            waitpid(pid, &status, WUNTRACED);

        }

    }
}