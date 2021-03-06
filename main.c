#include "def.h"

void CleanUpCommand(char *str) {
    int i = 0;
    char output[max_size];
    int output_iterator = 0;
    int spaceAddedBefore = 0;
    while (str[i] != '\0') {
        if (str[i] != ' ' && str[i] != '\t') {
            output[output_iterator++] = str[i++];
            spaceAddedBefore = 0;
            continue;
        }

        if (str[i] == ' ') {
            if (spaceAddedBefore == 1) {
                i++;
                continue;
            } else {
                output[output_iterator++] = str[i++];
                spaceAddedBefore = 1;
                continue;
            }
        }

        if (str[i] == '\t') {
            if (spaceAddedBefore == 1) {
                i++;
                continue;
            } else {
                output[output_iterator++] = ' ';
                ++i;
                spaceAddedBefore = 1;
                continue;
            }
        }

    }
    output[output_iterator] = '\0';

    int start = 0;

    while (output[start] != '\0' && (output[start] == ' ' || output[start] == '\t')) {
        ++start;
    }

    int end = strlen(output) - 1;

    while (end >= 0 && (output[end] == ' ' || output[start] == '\t')) {
        --end;
    }

    char new_output[max_size];

    int iter = 0;

    for (int i = start; i <= end; ++i) {
        new_output[iter++] = output[i];
    }
    new_output[iter] = '\0';
    strcpy(str, new_output);
}


void runCommand(char *currCommand) {

    if (history->NumEle < 20)
        Inject(history, currCommand);
    else {
        Pop(history);
        Inject(history, currCommand);
    }

    char currCommandName[max_size];
    unsigned long int length = strlen(currCommand);
    unsigned long int i = 0;
    while (currCommand[i] != ' ' && i < length) {
        currCommandName[i] = currCommand[i];
        i++;
    }
    currCommandName[i] = '\0';

    if (strcmp(currCommandName, "exit") == 0) {
        write_history();
        exit(1);
    } else if (isPipe(currCommand) == 1) {
        isPi = 1;
        piping(currCommand);
        isPi = 0;
    } else if (isRedirec(currCommand) == 1) {
        isRe = 1;
        io_redirection(currCommand);
        isRe = 0;
    } else if (strcmp(currCommandName, "echo") == 0)
        echo(currCommand);
    else if (strcmp(currCommandName, "pwd") == 0) {
        pwd();
    } else if (strcmp(currCommandName, "cd") == 0)
        cd(currCommand, homeDir);
    else if (strcmp(currCommandName, "pinfo") == 0)
        pinfo(currCommand);
    else if (strcmp(currCommandName, "ls") == 0)
        ls(currCommand, homeDir);
    else if (strcmp(currCommandName, "repeat") == 0)
        repeat(currCommand);
    else if (strcmp(currCommandName, "history") == 0)
        getHistory(currCommand);
    else if (strcmp(currCommandName, "jobs") == 0)
        jobs(currCommand);
    else if (strcmp(currCommandName, "sig") == 0)
        sig(currCommand);
    else if (strcmp(currCommandName, "fg") == 0)
        fg_command(currCommand);
    else if (strcmp(currCommandName, "bg") == 0)
        bg_command(currCommand);
    else
        system_commands(currCommand);
}

int main(void) {
    isRe = 0;
    isPi = 0;
    saved_stdout = dup(STDOUT_FILENO); //saved_stdout refers to the STDOUT
    saved_stdin = dup(STDIN_FILENO);
    shell_pid = getpid();
    signal(SIGCHLD, SIGCHLD_handler);
    signal(SIGINT, SIGINT_handler);
    signal(SIGTSTP, SIGTSTP_handler);

    //-------Initialising the global variables------//

    strcpy(lastAddedCommand, ""); //For removing repetitive commands in history

    totalBgProc = 0; //Keeps track of current number of active bg processes
    numJobs = 0;

    history = initQueue(); //Data Structure (Queue) that stores latest executed commands / history

    getcwd(homeDir, max_size); // Pseudo home directory for our shell

    load_history(); //Loads the history for past sessions from history.txt file

    strcpy(lastVisitedDir, ""); // For (cd -)

    char *command = (char *) malloc(max_size * sizeof(char));

    size_t x;
    x = max_size;

    char *token;
    const char s[2] = ";";      //deliminator

    char currCommand[max_size];

    while (1) {
        prompt(homeDir);

        int err = getline(&command, &x, stdin);
        // getline return -1 on failure to read a line (including end-of-file condition)
        if (err == -1) {
            print_red();
            printf("\nLogging Out\n");
            print_reset();
            write_history();
            exit(1);
        }

        if (command[strlen(command) - 1] == '\n')
            command[strlen(command) - 1] = '\0'; //REMOVE LAST '\n' CHARACTER

        token = strtok(command, ";");

        char commands[strlen(command)][max_size];
        int itr = 0;

        while (token != NULL) {
            strcpy(commands[itr++], token);
            token = strtok(NULL, ";");
        }

        for (int i = 0; i < itr; i++) {
            strcpy(currCommand, commands[i]);
            CleanUpCommand(currCommand); //Removes all the extra spaces and tabs
            runCommand(currCommand);
        }
    }
}
