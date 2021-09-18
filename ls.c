#include "def.h"
int processCommand(char* command, char dirName[max_size][max_size], int* dir_size, int* isA, int* isL, char* currDir, char* home) {

    char params[100][100];
    int size = 0;

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

    if(size == 0) {

        return 0;
    }

    if(strcmp(params[0], "ls") != 0) {

        return 0;
    }

    if(strcmp(command, "ls")==0)
    {

        strcpy(dirName[0],currDir);
        (*dir_size) +=1 ;
        return 1;
    }

    int dir_flag = 0;

    for(int i = 1; i < size; ++ i)
    {

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

            dir_flag = 1;
            if(strcmp(p, ".") == 0) {
                strcpy(dirName[*dir_size], currDir);
                (*dir_size)+=1;
                continue;
            }
            else if(strcmp(p, "..") == 0) {
                //char parent[max_size];
                //findParent(parent, currDir);
                strcpy(dirName[*dir_size], "..");
                (*dir_size)+=1;
            }
            else if(strcmp(p, "~") == 0) {
                strcpy(dirName[(*dir_size)], home);
                (*dir_size)+=1;

            }
            else
            {
                strcpy(dirName[(*dir_size)], p);
                (*dir_size)+=1;
            }

        }
    }
    if(dir_flag == 0)
    {
        (*dir_size)+=1;
        strcpy(dirName[0],currDir);

        return 1;
    }
    return 1;
}

int isDirectory(const char *path)
{
    struct stat statBuf;
    if (stat(path, &statBuf) != 0)
        return 0;

    return S_ISDIR(statBuf.st_mode);
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

struct LSL_info{

    char permissions[11];
    unsigned int numLinks;

    unsigned int userID;
    unsigned int groupID;
    char userName[max_size];
    char groupName[max_size];

    unsigned long int size;
    char date[max_size];
    unsigned long int numBlocks;

};


int getFileDetails(char* fileName, struct LSL_info* fileInfo)
{
    struct stat statbuf;
    struct passwd *p;
    struct group *g;

    int x = stat(fileName, &statbuf);
    if(x != 0)
    {
        printf(RED "Error: %s: Invalid File/Directory name\n", fileName);
        return 0;
    }
    fileInfo->userID = statbuf.st_uid;
    fileInfo->groupID = statbuf.st_gid;
    p = getpwuid(fileInfo->userID);
    strcpy(fileInfo->userName, p->pw_name);
    g = getgrgid(fileInfo->groupID);
    strcpy(fileInfo->groupName, g->gr_name);

    fileInfo->numLinks = statbuf.st_nlink;
    fileInfo->size = statbuf.st_size;
    struct timespec t;
    t = statbuf.st_ctim;

    struct tm *properFormatTime;
    properFormatTime = localtime(&t.tv_sec);

    int fileMonth = properFormatTime->tm_mon + 1;
    int fileYear = properFormatTime->tm_year +1900;
    time_t tn = time(0);
    struct tm* now = localtime(&tn);

    int currYear = now->tm_year + 1900;
    int currMonth = now->tm_mon + 1;

    if(currYear > fileYear)
    {
        strftime(fileInfo->date, 24, "%b   %d   %Y", localtime(&(statbuf.st_ctime)));
    }
    else
    {
        if(currMonth - fileMonth > 6)
            strftime(fileInfo->date, 24, "%b   %d   %Y", localtime(&(statbuf.st_ctime)));
        else
            strftime(fileInfo->date, 24, "%b   %d   %H:%M", localtime(&(statbuf.st_ctime)));
    }


    getPermissions(statbuf, fileInfo->permissions);
    fileInfo->numBlocks = statbuf.st_blocks;
    return 1;

}

void ls(char* command, char* home)
{
    char currDir[max_size];
    getcwd(currDir, max_size);

    int isA = 0;
    int isL = 0;

    char dirName[max_size][max_size]; //MULTIPLE DIRECTORY/FILE NAMES
    int dir_size = 0;

    int x = 0;
    x = processCommand(command, dirName, &dir_size, &isA, &isL, currDir, home);

    if(x == 0)
        printf(RED "Invalid Command\n");

    long unsigned int totalBlocks;
    char fileName[max_size];

   for( int y = 0; y < dir_size ; y++)
   {
       //TRAVERSING THROUGH THE DIR/FILE NAMES

       if(dir_size > 1 && isDirectory(dirName[y]) == 1)
           printf(GREEN "%s:\n",dirName[y]);

       if ( isDirectory(dirName[y]) == 1)
       {
           totalBlocks = 0;
           DIR *d;
           struct dirent *dir;
           d = opendir(dirName[y]);

           if (d)
           {
               if (isL == 0)
               {
                   while ((dir = readdir(d)) != NULL)
                       if (dir->d_name[0] != '.' || (dir->d_name[0] == '.' && isA == 1))
                           printf(GREEN "%s\n", dir->d_name);
               }
               else
               {
                   while ((dir = readdir(d)) != NULL)
                       if (dir->d_name[0] != '.' || (dir->d_name[0] == '.' && isA == 1))
                       {
                           strcpy(fileName, dirName[y]);
                           strcat(fileName, "/");
                           strcat(fileName, dir->d_name);
                           fileName[strlen(dirName[y]) + strlen(dir->d_name) + 1] = '\0';

                           struct LSL_info fileInfo;
                           getFileDetails(fileName, &fileInfo);

                           printf(GREEN  "%s  %d  %s  %s  %11ld  %6s  %s\n", fileInfo.permissions, fileInfo.numLinks, fileInfo.userName,
                                  fileInfo.groupName,
                                  fileInfo.size, fileInfo.date, dir->d_name);

                           totalBlocks = totalBlocks + fileInfo.numBlocks;
                       }

                   printf(GREEN "Total %ld\n", totalBlocks / 2);
               }
               closedir(d);

           } else
               printf(RED "No such directory exists\n");
       }
       else
       {
           //dirName[y] is just a file

           if (isL == 0)
           {
               struct stat statbuf;
               int y = stat(fileName, &statbuf);
               if(y != 0)
               {
                   printf(RED "Error: %s: Invalid File/Directory name\n", fileName);
                   return;
               }

               printf(GREEN "%s\n", dirName[y]);
           }
           else
           {
               if (dirName[y][0] != '.' || (dirName[y][0] == '.' && isA == 1))
               {
                   strcpy(fileName, dirName[y]);
                   struct LSL_info fileInfo;
                   int x = 0;
                   x = getFileDetails(fileName, &fileInfo);
                   if(x == 0)
                       return;

                   printf(GREEN "%s  %d  %s  %s  %11ld  %6s  %s\n", fileInfo.permissions, fileInfo.numLinks, fileInfo.userName,
                          fileInfo.groupName,
                          fileInfo.size, fileInfo.date, dirName[y]);
               }
           }
       }
   }
}