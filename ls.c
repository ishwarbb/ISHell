#include "headers.h"
#include "functions.h"

signed cmp(const void *a, const void *b)
{
    struct dirent **x = (struct dirent **)a;
    struct dirent **y = (struct dirent **)b;
    return strcasecmp((*x)->d_name, (*y)->d_name);
}


char *lscolourblue(char *file)
{
    char *x = (char *)malloc(MY_LEN * sizeof(char));
    strcpy(x, "\033[1;34m");
    strcat(x, file);
    strcat(x, "\033[0m");

    strcpy(file, x);
    free(x);

    return file;
}

char *lscolourwhite(char *file)
{
    char *x = (char *)malloc(MY_LEN * sizeof(char));
    strcpy(x, "\033[1;37m");
    strcat(x, file);
    strcat(x, "\033[0m");

    strcpy(file, x);
    free(x);

    return file;
}

char *lscolourgreen(char *file)
{
    char *x = (char *)malloc(MY_LEN * sizeof(char));
    strcpy(x, "\033[1;32m");
    strcat(x, file);
    strcat(x, "\033[0m");

    strcpy(file, x);
    free(x);

    return file;
}

char *lscolouryellow(char *file)
{
    char *x = (char *)malloc(MY_LEN * sizeof(char));
    strcpy(x, "\033[1;35m");
    strcat(x, file);
    strcat(x, "\033[0m");

    strcpy(file, x);
    free(x);

    return file;
}

void lssize(char* dir, int lsl_flag, int lsa_flag)
{
    int blocksize = 0;

    if (debug)
        printf("before tildify %s \n", dir);
    DeTildify(dir);

    DIR* dirp = opendir(dir);
    if (dirp == NULL)
    {
        perror("Error ls :");
        return;
    }
    struct dirent *d = readdir(dirp);
    struct dirent *files[MY_LEN];
    int totalFiles = 0;
    while (d != NULL)
    {
        files[totalFiles] = d;
        // printf("%s\n",d->d_name);
        totalFiles++;
        d = readdir(dirp);
    }

    for (int i = 0; i < totalFiles; i++)
    {
        if (lsa_flag == 0)
        {
            if (files[i]->d_name[0] == '.')
                continue;
        }

        struct stat stats;
        char filePath[MY_LEN];
        strcpy(filePath, dir);
        strcat(filePath, "/");
        strcat(filePath, files[i]->d_name);

        int x = stat(filePath, &stats);
        if (x != 0)
        {
            continue;
            perror("Stat in ls dir : ");
            printf("Was trying to access %s \n",filePath);
        }

        blocksize += (stats.st_blocks * 512 + 1023) / 1024;

    }
    closedir(dirp);

    printf("total %d\n",blocksize);

}

void lsdir(char *dir, int lsl_flag, int lsa_flag)
{
    if(lsl_flag == 1) lssize(dir,lsl_flag,lsa_flag);

    DIR *dirp = opendir(dir);
    if (dirp == NULL)
    {
        perror("Error ls :");
        return;
    }
    struct dirent *d = readdir(dirp);
    if (debug)
        perror("ls cmd \n");
    struct dirent *files[MY_LEN];
    int totalFiles = 0;
    while (d != NULL)
    {
        files[totalFiles] = d;
        // printf("%s\n",d->d_name);
        totalFiles++;
        d = readdir(dirp);
    }

    qsort(files, totalFiles, sizeof(struct dirent *), cmp);
    if (debug)
        printf("after sorted\n");

    // if (lsl_flag == 0)
    // {
    //     if (debug)
    //         printf("token is null\n");
    //     for (int i = 0; i < totalFiles; i++)
    //     {
    //         if (lsa_flag == 0)
    //         {
    //             if (files[i]->d_name[0] == '.')
    //                 continue;
    //         }
    //         printf("%s ", files[i]->d_name);
    //     }
    //     printf("\n");
    // }
    // else if (lsl_flag == 1)
    // {
    if (debug)
        printf("token is -l\n");
    for (int i = 0; i < totalFiles; i++)
    {
        if (lsa_flag == 0)
        {
            if (files[i]->d_name[0] == '.')
                continue;
        }

        int w = 0;
        int b = 0;
        int g = 0;
        int l = 0;

        struct stat stats;
        char filePath[MY_LEN];
        strcpy(filePath, dir);
        strcat(filePath, "/");
        strcat(filePath, files[i]->d_name);

        int x = lstat(filePath, &stats);
        if (x != 0)
        {
            continue;
            perror("Stat in ls dir : ");
            printf("Was trying to access %s \n",filePath);
        }

        char permission[MY_LEN] = "";
        if (files[i]->d_type == DT_REG)
        {
            strcpy(permission, "-");
            w = 1;
        }
        else if (files[i]->d_type == DT_DIR)
        {
            strcpy(permission, "d");
            b = 1;
        }
        else if(files[i]->d_type == DT_LNK )
        {
            strcpy(permission, "l");
        }
        else
        {
            if (debug)
                printf("this file %s is of %d type\n",files[i]->d_name, files[i]->d_type);
            continue;
        }
        /// Add more here

        int exe = 0;

        int userReadPerm = stats.st_mode & S_IRUSR;
        int userWritePerm = stats.st_mode & S_IWUSR;
        int userExecutePerm = stats.st_mode & S_IXUSR;

        userReadPerm ? strcat(permission, "r") : strcat(permission, "-");
        userWritePerm ? strcat(permission, "w") : strcat(permission, "-");
        userExecutePerm ? strcat(permission, "x") : strcat(permission, "-");

        int groupReadPerm = stats.st_mode & S_IRGRP;
        int groupWritePerm = stats.st_mode & S_IWGRP;
        int groupExecutePerm = stats.st_mode & S_IXGRP;

        groupReadPerm ? strcat(permission, "r") : strcat(permission, "-");
        groupWritePerm ? strcat(permission, "w") : strcat(permission, "-");
        groupExecutePerm ? strcat(permission, "x") : strcat(permission, "-");

        int othersReadPerm = stats.st_mode & S_IROTH;
        int othersWritePerm = stats.st_mode & S_IWOTH;
        int othersExecutePerm = stats.st_mode & S_IXOTH;

        othersReadPerm ? strcat(permission, "r") : strcat(permission, "-");
        othersWritePerm ? strcat(permission, "w") : strcat(permission, "-");
        othersExecutePerm ? strcat(permission, "x") : strcat(permission, "-");

        if (permission[0] == '-')
        {
            if (permission[3] == 'x' || permission[6] == 'x' || permission[9] == 'x')
            {
                // printf("uwu");
                // lscolourgreen(files[i]->d_name);
                g = 1;
            }
        }

        int noOfLinks = stats.st_nlink;
        char *OwnerName = getpwuid(stats.st_uid)->pw_name;
        char *Group = getpwuid(stats.st_gid)->pw_name;
        int filesize = stats.st_size;
        time_t lastChange = stats.st_mtime;
        struct tm *ptm = localtime(&lastChange);
        char LastChange[MY_LEN];
        strftime(LastChange, MY_LEN, "%b %d %R", ptm);
        if (debug)
            printf("filesize = %d\n", filesize);

        if (lsl_flag == 0)
        {
            if (b)
                printf("%s ", lscolourblue(files[i]->d_name));
            else if (g)
                printf("%s ", lscolourgreen(files[i]->d_name));
            else if (w)
                printf("%s ", lscolourwhite(files[i]->d_name));
            else if (l)
                printf("%s ", lscolouryellow(files[i]->d_name));
        }
        else
        {
            if (b)
                printf("%s %3d %s %s %6d %s %s\n", permission, noOfLinks, OwnerName, Group, filesize, LastChange, lscolourblue(files[i]->d_name));
            else if (g)
                printf("%s %3d %s %s %6d %s %s\n", permission, noOfLinks, OwnerName, Group, filesize, LastChange, lscolourgreen(files[i]->d_name));
            else if (w)
                printf("%s %3d %s %s %6d %s %s\n", permission, noOfLinks, OwnerName, Group, filesize, LastChange, lscolourwhite(files[i]->d_name));
        }
    }

    if (lsl_flag == 0)
        printf("\n");
    // }
    closedir(dirp);
}

void lsfile(char *file, int lsl_flag, int lsa_flag)
{
    struct stat stats;
    char filePath[MY_LEN];
    strcpy(filePath, file);

    int g = 0;
    int w = 1;

    int x = lstat(filePath, &stats);
    if (x != 0)
    {
        return;
        perror("Stat in ls file : ");
    }

    char permission[MY_LEN] = "";
    strcpy(permission, "-");
    /// Add more here

    int userReadPerm = stats.st_mode & S_IRUSR;
    int userWritePerm = stats.st_mode & S_IWUSR;
    int userExecutePerm = stats.st_mode & S_IXUSR;

    userReadPerm ? strcat(permission, "r") : strcat(permission, "-");
    userWritePerm ? strcat(permission, "w") : strcat(permission, "-");
    userExecutePerm ? strcat(permission, "x") : strcat(permission, "-");

    int groupReadPerm = stats.st_mode & S_IRGRP;
    int groupWritePerm = stats.st_mode & S_IWGRP;
    int groupExecutePerm = stats.st_mode & S_IXGRP;

    groupReadPerm ? strcat(permission, "r") : strcat(permission, "-");
    groupWritePerm ? strcat(permission, "w") : strcat(permission, "-");
    groupExecutePerm ? strcat(permission, "x") : strcat(permission, "-");

    int othersReadPerm = stats.st_mode & S_IROTH;
    int othersWritePerm = stats.st_mode & S_IWOTH;
    int othersExecutePerm = stats.st_mode & S_IXOTH;

    othersReadPerm ? strcat(permission, "r") : strcat(permission, "-");
    othersWritePerm ? strcat(permission, "w") : strcat(permission, "-");
    othersExecutePerm ? strcat(permission, "x") : strcat(permission, "-");

    if (permission[0] == '-')
    {
        if (permission[3] == 'x' || permission[6] == 'x' || permission[9] == 'x')
        {
            g = 1;
        }
    }

    int noOfLinks = stats.st_nlink;
    char *OwnerName = getpwuid(stats.st_uid)->pw_name;
    char *Group = getpwuid(stats.st_gid)->pw_name;
    int filesize = stats.st_size;
    time_t lastChange = stats.st_mtime;
    struct tm *ptm = localtime(&lastChange);
    char LastChange[MY_LEN];
    strftime(LastChange, MY_LEN, "%b %d %R", ptm);
    if (debug)
        printf("filesize = %d\n", filesize);

    if (lsl_flag == 0)
    {
        if (g)
            printf("%s\n", lscolourgreen(file));
        else
            printf("%s\n", file);
        return;
    }

    if (g)
        printf("%s %3d %s %s %6d %s %s\n", permission, noOfLinks, OwnerName, Group, filesize, LastChange, lscolourgreen(filePath));
    else if (w)
        printf("%s %3d %s %s %6d %s %s\n", permission, noOfLinks, OwnerName, Group, filesize, LastChange, lscolourwhite(filePath));
}

int lscmp(const void* x, const void* y)
{
    char** a = (char**) x;
    char** b = (char**) y;
    return strcasecmp(*a,*b );
}

void ls(char *path, char *command)
{

    ////////////// TOTAL

    if (debug)
        printf("ls Command = %s\n", command);
    char *token = strtok(command, " \t");
    if (debug)
        printf("no more tok after this one - %s\n", token);
    if (strcmp(token, "ls") != 0)
    {
        printf("this is not an ls command \n");
    }
    int lsl_flag = 0;
    int lsa_flag = 0;

    char *lspaths[MY_LEN];
    int totallspaths = 0;
    int otherpaths = 0;

    token = strtok(NULL, " \t");
    while (token != NULL)
    {
        if (debug)
            printf("token = :%s\n", token);
        if (debug)
            printf("uwu\n");

        if (strcmp(token, "-l") == 0)
            lsl_flag = 1;
        else if (strcmp(token, "-a") == 0)
            lsa_flag = 1;
        else if (strcmp(token, "-al") == 0)
        {
            lsa_flag = 1;
            lsl_flag = 1;
        }
        else if (strcmp(token, "-la") == 0)
        {
            lsa_flag = 1;
            lsl_flag = 1;
        }
        else if (token != NULL)
        {
            otherpaths = 1;
            lspaths[totallspaths] = token;
            totallspaths++;
            if (debug)
                printf("otherpaths detected for %s now totallspaths = %d ans set to %s\n", token, totallspaths, lspaths[totallspaths - 1]);
        }
        token = strtok(NULL, " \t");
        if (debug)
            printf("all tokens traversed\n");
    }

    if (debug)
    {
        printf("%s --- %s\n", lspaths[0], lspaths[1]);
    }

    if (otherpaths == 0)
    {
        lsdir(path, lsl_flag, lsa_flag);
    }
    else
    {
        // paths here need to be DeTidified
        if (debug)
            printf("totallspaths = %d\n", totallspaths);

        qsort(lspaths,totallspaths, sizeof(char*),lscmp);

        for (int i = 0; i < totallspaths; i++)
        {
            char lspath[MY_LEN];
            strcpy(lspath, lspaths[i]);
            if (debug)
                printf("before tildify %s \n", lspath);
            DeTildify(lspath);
            if (totallspaths != 1)
            {
                if(CheckValidDir(lspath,path)){
                printf("%s:\n", lspath);
                }
            }
            if (debug)
                printf("for lspath %s\n", lspath);
            if (CheckValidDir(lspath, path))
            {
                if (debug)
                    printf("lspath is dir\n");
                lsdir(lspath, lsl_flag, lsa_flag);
            }
            else if (CheckValidFile(lspath, path))
            {
                if (debug)
                    printf("lspath is file\n");
                lsfile(lspath, lsl_flag, lsa_flag);
            }
            else
            {
                printf("Error: No such file or dirctory found\n");
                // return;
            }
            if (i != (totallspaths - 1))
               printf("\n"); //// Dodgy
        }
    }
}
