#include "def.h"
int processCommand(char* command, char dirName[max_size][max_size], int* dir_size, int* isA, int* isL, char* currDir, char* home) {
    char params[100][100];
    int size = 0;
    dir_size = 0;

    for(int i = 0; command[i] != '\0'; ++i) {
        if(command[i] == ' ') {
            continue;
        }
        int j = i;
        for(j = i; command[j] != '\0' && command[j] != ' '; ++j){
            params[size][j - i] = command[j];
        }
        params[size][j - i] = '\0';
        size++;
        if(command[j] == '\0') {
            break;
        }
        else {
            i = j;
        }
    }

    if(size > 4 || size == 0) {
        return 0;
    }

    if(strcmp(params[0], "ls") != 0) {
        return 0;
    }

    for(int i = 1; i < size; ++ i) {
        char * p = params[i];

        if(p[0] != '\0' && p[0] == '-') {

            for(int j = 1; p[j] != '\0'; ++j) {
                if(p[j] == 'l') {
                    *isL = 1;
                }
                else if(p[j] == 'a') {
                    *isA = 1;
                }
                else {
                    return 0;
                }
            }
        }
        else {
            if(strcmp(p, ".") == 0) {
                continue;
            }
            else if(strcmp(p, "..") == 0) {
                char parent[max_size];
                findParent(parent, currDir);
                strcpy(dirName[(*dir_size)++], parent);
            }
            else if(strcmp(p, "~") == 0) {
                strcpy(dirName[(*dir_size)++], home);
            }
            else{
                strcpy(dirName[(*dir_size)++], p);
            }

        }
    }
    return 1;
}

void getPermissions(struct stat buf, char perm[11])
{
    strcpy(perm,"----------");
    if(buf.st_mode & S_IFDIR)
        perm[0] = 'd';
    if(buf.st_mode & S_IRUSR)
        perm[1] = 'r';
    if(buf.st_mode & S_IWUSR)
        perm[2] = 'w';
    if(buf.st_mode & S_IXUSR)
        perm[3] = 'x';
    if(buf.st_mode & S_IRGRP)
        perm[4] = 'r';
    if(buf.st_mode & S_IWGRP)
        perm[5] = 'w';
    if(buf.st_mode & S_IXGRP)
        perm[6] = 'x';
    if(buf.st_mode & S_IROTH)
        perm[7] = 'r';
    if(buf.st_mode & S_IWOTH)
        perm[8] = 'w';
    if(buf.st_mode & S_IXOTH)
        perm[9] = 'x';
    perm[10] = '\0';

}

void ls(char* command, char* home)
{
    char currDir[max_size];
    getcwd(currDir, max_size);
   int isA = 0;
   int isL = 0;

   char dirName[max_size][max_size]; //MULTIPLE DIRECTORY NAMES
   int dir_size = 0;

   processCommand(command, dirName, &dir_size, &isA, &isL, currDir, home);
   char dir_now[max_size];

   for(int y = 0; y < dir_size ; y++) {
       DIR *d;
       struct dirent *dir;
       strcpy(dirName[y],dir_now);
       d = opendir(dir_now);
       printf(GREEN "%s\n",dir_now);

       if (d) {

           if (isL == 0)
           {
               while ((dir = readdir(d)) != NULL)
                   if (dir->d_name[0] != '.' || (dir->d_name[0] == '.' && isA == 1))
                       printf(GREEN "%s\n", dir->d_name);
           }
           else
           {
               long unsigned int totalBlocks = 0;
               char permissions[11];
               unsigned int numLinks = 0;
               unsigned int userID;
               unsigned int groupID;
               char userName[max_size];
               char groupName[max_size];
               unsigned int size = 0;
               char date[max_size];
               char fileName[max_size];
               struct stat statbuf;
               struct passwd *p;
               struct group *g;
               struct timespec t;  // Time of last modification
               struct tm *properFormatTime;


               while ((dir = readdir(d)) != NULL)
                   if (dir->d_name[0] != '.' || (dir->d_name[0] == '.' && isA == 1)) {
                       strcpy(fileName, dirName);
                       strcat(fileName, "/");
                       strcat(fileName, dir->d_name);
                       fileName[strlen(dirName) + strlen(dir->d_name) + 1] = '\0';

                       int x = stat(fileName, &statbuf);

                       userID = statbuf.st_uid;
                       groupID = statbuf.st_gid;
                       p = getpwuid(userID);
                       strcpy(userName, p->pw_name);
                       g = getgrgid(groupID);
                       strcpy(groupName, g->gr_name);

                       numLinks = statbuf.st_nlink;
                       size = statbuf.st_size;
                       t = statbuf.st_mtim;
                       properFormatTime = localtime(&t.tv_sec);
                       strftime(date, 24, "%b  %d  %H:%M", properFormatTime);

                       getPermissions(statbuf, permissions);
                       totalBlocks = totalBlocks + statbuf.st_blocks;
                       printf("\x1B[32m"  "%s  %d  %s  %s  %d  %s  %s\n", permissions, numLinks, userName, groupName,
                              size, date, dir->d_name);

                   }

               printf("Total %ld\n", totalBlocks / 2);
           }
           closedir(d);
       }
   }
}