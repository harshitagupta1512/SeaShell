#include "def.h"

void load_history() {
    char fileName[max_size];
    strcpy(fileName, homeDir);
    strcat(fileName, "/history.txt");

    FILE *fp = NULL;
    fp = fopen(fileName, "r+");

    char line[max_size];
    if (fp == NULL) {
        print_red();
        printf("Error: Can't load history for past sessions\n");
        print_reset();
        return;
    }

    while (fgets(line, sizeof(line), fp) != NULL) {
        //printf("%s command added in history queue\n", line);
        if (line[strlen(line) - 1] == '\n')
            line[strlen(line) - 1] = '\0'; //Remove the \n charcter from the end of the command
        Inject(history, line);
    }

    fclose(fp);
}

void write_history() {
    char fileName[max_size];
    strcpy(fileName, homeDir);
    strcat(fileName, "/history.txt");

    FILE *fp = NULL;
    fp = fopen(fileName, "w");
    if (fp == NULL) {
        print_red();
        printf("Error: Can't write history for past sessions\n");
        print_reset();
        return;
    }

    PtrQueueEle Curr = history->Front;

    if (history->NumEle != 0) {
        while (Curr->next != NULL) {
            fputs(Curr->data, fp);
            fputc('\n', fp);
            Curr = Curr->next;
        }
        fputs(Curr->data, fp);
        fputc('\n', fp);
    }


    fclose(fp);

}

void getHistory(char *command) {
    if (strcmp(command, "history") == 0)
        printQueue(history, 10);
    else if (countSpaces(command) != 1) {
        print_red();
        printf("Error :- Invalid number of arguments\n");
        print_reset();
        return;
    } else {
        unsigned int i = 8;
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
            printf("Error: %s: Invalid argument for history\n", number);
            print_reset();
            return;
        }

        printQueue(history, num);
    }
}