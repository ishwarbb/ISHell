#include "headers.h"
#include "functions.h"


void ddir(char *path, int f_flag, int d_flag)
{
    if (debug)
        printf("performing ddir on %s\n", path);
    DIR *dirp = opendir(path);
    if (dirp == NULL)
    {
        perror("Error ddir");
        printf("the dir was %s \n",path);
        closedir(dirp);
        return;
    }
    struct dirent *d = readdir(dirp);
    if (debug)
        perror("ddir cmd ");
    struct dirent *contents[MY_LEN];
    for (int i = 0; i < MY_LEN; i++)
    {
        contents[i] = (struct dirent *)malloc(sizeof(struct dirent *));
    }
    int totalContents = 0;
    while (d != NULL)
    {
        assert(totalContents < MY_LEN);
        contents[totalContents] = d;
        // printf("%s\n",d->d_name);
        totalContents++;
        d = readdir(dirp);
    }

    if (debug)
        printf("e0\n");
    if (d_flag)
        printf("%s\n", Dotify(path));
    if (debug)
        printf("e0\n");

    for (int i = 0; i < totalContents; i++)
    {
        if (debug)
            printf("e1\n");
        char content[MY_LEN];
        strcpy(content, contents[i]->d_name);
        if (debug)
            printf("e2\n");

        if (!strcmp(content, ".."))
            continue;
        if (!strcmp(content, "."))
            continue;

        if (debug)
            printf("content = %s\n", content);

        char x[MY_LEN];
        strcpy(x, path);
        strcat(x, "/");
        strcat(x, content);

        if (debug)
            printf("x content = %s\n", x);

        if (CheckValidDir(x, path))
        {
            if (debug)
                printf("x-content %s is a dir\n", x);
            if (debug)
                printf("===================== entering recursion of %s\n", x);
            ddir(x, f_flag, d_flag);
            if (debug)
                printf("===================== exiting recursion of %s\n\n\n", x);
        }
        else
        {
            if (debug)
                printf("x-content %s is a file \n", x);
            if (f_flag)
                printf("%s\n", Dotify(x));
        }
    }
    closedir(dirp);
}

void dfile(char *path, char *filename, char *cwd, int f_flag, int d_flag)
{
    if (debug)
    {
        printf("performing ddir on %s\n", path);
        printf("cwd === %s\n", cwd);
        printf("filename === %s\n", filename);
    }
    DIR *dirp = opendir(path);
    if (dirp == NULL)
    {
        perror("Error dfile ");
        closedir(dirp);
        return;
    }
    struct dirent *d = readdir(dirp);
    if (debug)
        perror("ddir cmd ");
    struct dirent *contents[MY_LEN];
    for (int i = 0; i < MY_LEN; i++)
    {
        contents[i] = (struct dirent *)malloc(sizeof(struct dirent *));
    }
    int totalContents = 0;
    while (d != NULL)
    {
        assert(totalContents < MY_LEN);
        contents[totalContents] = d;
        // printf("%s\n",d->d_name);
        totalContents++;
        d = readdir(dirp);
    }

    // printf(":%s\n", path);
    if (d_flag)
        if (!strcmp(cwd, filename))
            printf("%s\n", Dotify(path));

    for (int i = 0; i < totalContents; i++)
    {
        char content[MY_LEN];
        strcpy(content, contents[i]->d_name);

        if (!strcmp(content, ".."))
            continue;
        if (!strcmp(content, "."))
            continue;

        if (debug)
            printf("content = %s\n", content);

        char x[MY_LEN];
        strcpy(x, path);
        strcat(x, "/");
        strcat(x, content);

        if (debug)
            printf("x content = %s\n", x);

        if (CheckValidDir(x, path))
        {
            if (debug)
                printf("x-content %s is a dir\n", x);
            if (debug)
                printf("===================== entering recursion of %s\n", x);
            dfile(x, filename, content, f_flag, d_flag);
            if (debug)
                printf("===================== exiting recursion of %s\n\n\n", x);
        }
        else
        {
            if (debug)
                printf("x-content %s is a file \n", x);
            if (f_flag)
                if (!strcmp(content, filename))
                    printf("%s\n", Dotify(x));
        }
    }
    closedir(dirp);
}

void discover(char *path, char *command)
{
    if (debug)
        printf("discover path = %s\n", path);
    if (debug)
        printf("discover commmand = %s\n", command);
    char *token = strtok(command, " \t");
    if (debug)
        printf("no more tok after this one - %s\n", token);
    if (strcmp(token, "discover") != 0)
    {
        printf("this is not a discover command \n");
    }

    int f_flag = 0;
    int d_flag = 0;

    int file = 0;
    char filename[MY_LEN] = "";
    int target = 0;
    char targetdir[MY_LEN] = "";
    strcpy(targetdir, path);

    token = strtok(NULL, " \t");
    while (token != NULL)
    {
        if (debug)
            printf("token = :%s\n", token);
        if (debug)
            printf("uwu\n");

        if (strcmp(token, "-f") == 0)
            f_flag = 1;
        else if (strcmp(token, "-d") == 0)
            d_flag = 1;
        else if (strcmp(token, "-fd") == 0)
        {
            d_flag = 1;
            f_flag = 1;
        }
        else if (strcmp(token, "-df") == 0)
        {
            d_flag = 1;
            f_flag = 1;
        }
        else if (token != NULL)
        {
            if (HasQuotes(token))
            {
                if (file == 1)
                {
                    printf("Error: Too many Arguments for discover\n");
                    return;
                }
                else
                {
                    file = 1;
                    strcpy(filename, token);
                    removeQuotes(filename);
                }
            }
            else
            {
                if (target == 1)
                {
                    printf("Error: Too many Arguments for discover\n");
                    return;
                }
                else
                {
                    target = 1;
                    strcpy(targetdir, token);
                    if (debug)
                        printf("targetdir = %s", targetdir);
                    if(strcmp(targetdir,"..") != 0)
                        DeDotify(targetdir);
                    // AbsPathofDir(targetdir,path);
                }
            }
        }

        token = strtok(NULL, " \t");
        if (debug)
            printf("all tokens traversed\n");
    }

    if ((f_flag == 0) && (d_flag == 0))
    {
        f_flag = 1;
        d_flag = 1;
    }

    if (file == 1)
    {
        dfile(targetdir, filename, targetdir, f_flag, d_flag);
    }
    else
    {
        ddir(targetdir, f_flag, d_flag);
    }
}