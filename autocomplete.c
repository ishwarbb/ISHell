#include "headers.h"
#include "functions.h"

void die(const char *s)
{
    perror(s);
    exit(1);
}

struct termios orig_termios;

void disableRawMode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

void enableRawMode()
{
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
        die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
        die("tcsetattr");
}

char *get_line()
{
    char *retval = (char *)malloc(MY_LEN * sizeof(char));
    int ri = 0;
    char c;

    char *currpath = (char *)malloc(MY_LEN * sizeof(char));
    char *ret = getcwd(currpath, MY_LEN);
    if (debug)
    {
        printf("getcwd in get_line returned %s\n", ret);
        perror("getcwd in get_line");
    }

    enableRawMode();
    while (read(0, &c, 1) == 1)
    {
        // if(debug) printf("get_line c = %c",c);
        if (iscntrl(c))
        {
            if (c == 10)
            {
                printf("\n");
                if (debug)
                    printf("enter detected\n");
                break;
            }
            else if (c == 127)
            {
                if (retval[ri - 1] == 9)
                {
                    for (int i = 0; i < 7; i++)
                    {
                        printf("\b");
                    }
                }
                retval[--ri] = '\0';
                printf("\b \b");
                fflush(stdout);
            }
            else if (c == 9)
            {
                autocomplete(retval, currpath);
            }
            else
            {
                printf("%c", c);
                fflush(stdout);
            }
        }
        else
        {
            retval[ri] = c;
            ri++;
            printf("%c", c);
            fflush(stdout);
        }
    }
    disableRawMode();
    strcat(retval, " ");
    return retval;
}

char **listOfFiles(char *currpath, char *incompletepath, char *prefix, char *command, int notboth)
{
    char buff[MY_LEN];
    char *ret = getcwd(buff, MY_LEN);

    char **x = (char **)malloc(MY_LEN * sizeof(char *));
    for (int i = 0; i < MY_LEN; i++)
    {
        x[i] = (char *)malloc(MY_LEN * sizeof(char));
    }

    DIR *dirp = opendir(currpath);
    if (dirp == NULL)
    {
        perror("Error ls :");
        return NULL;
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

    int matches = 0;
    char *allMatches[MY_LEN];
    for (int i = 0; i < totalFiles; i++)
    {
        struct stat stats;
        char filePath[MY_LEN] = "";
        strcpy(filePath, currpath);
        strcat(filePath, "/");
        strcat(filePath, files[i]->d_name);

        if(CheckValidFile(filePath,currpath) == 0) continue;

        if (debug)
            printf("debud mode = %s\n", filePath);

        strcpy(filePath,"");
        strcpy(filePath,files[i]->d_name);

        if (strncmp(filePath, incompletepath, strlen(incompletepath)) == 0)
        {
            allMatches[matches] = (char *)malloc(MY_LEN * sizeof(char));
            strcpy(allMatches[matches], filePath);
            matches++;
        }
    }

    if (matches == 1)
    {
        printf("\r");
        prompt(buff);
        printf("%s %s", prefix, allMatches[0]);
        fflush(stdout);
        strcpy(command, "");
        strcat(command, prefix);
        strcat(command, " ");
        strcat(command, allMatches[0]);
    }
    else if (matches > 0)
    {
        printf("\n");
        for (int i = 0; i < matches; i++)
        {
            printf("%s\n", allMatches[i]);
        }
        printf("\r");
        if(notboth) prompt(buff);
        printf("%s %s", prefix, incompletepath);
        fflush(stdout);
    }
}

char **listOfDir(char *currpath, char *incompletepath, char *prefix, char *command)
{
    char buff[MY_LEN];
    char *ret = getcwd(buff, MY_LEN);

    char **x = (char **)malloc(MY_LEN * sizeof(char *));
    for (int i = 0; i < MY_LEN; i++)
    {
        x[i] = (char *)malloc(MY_LEN * sizeof(char));
    }

    DIR *dirp = opendir(currpath);
    if (dirp == NULL)
    {
        perror("Error ls :");
        return NULL;
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

    int matches = 0;
    char *allMatches[MY_LEN];
    for (int i = 0; i < totalFiles; i++)
    {
        struct stat stats;
        char filePath[MY_LEN] = "";
        strcpy(filePath, currpath);
        strcat(filePath, "/");
        strcat(filePath, files[i]->d_name);

        if (CheckValidDir(filePath, currpath) == 0)
            continue;

        if (debug)
            printf("debud mode = %s\n", filePath);

        DeDotify(incompletepath);

        if (strncmp(filePath, incompletepath, strlen(incompletepath)) == 0)
        {
            allMatches[matches] = (char *)malloc(MY_LEN * sizeof(char));
            strcpy(allMatches[matches], filePath);
            matches++;
        }
    }

    if (matches == 1)
    {
        printf("\r");
        prompt(buff);
        printf("%s %s/", prefix, Dotify(allMatches[0]));
        fflush(stdout);
        strcpy(command, "");
        strcat(command, prefix);
        strcat(command, " ");
        strcat(command, Dotify(allMatches[0]));
        strcat(command, "/");
    }
    else if (matches > 0)
    {
        printf("\n");
        for (int i = 0; i < matches; i++)
        {
            printf("%s/\n", Dotify(allMatches[i]));
        }
        printf("\r");
        prompt(buff);
        printf("%s %s", prefix, Dotify(incompletepath));
        fflush(stdout);
    }
}

char *autocomplete(char *command, char *currpath)
{
    int cdflag = 0, catflag = 0, lsflag = 0;
    int i = 0;
    char temp[MY_LEN] = "";
    strcpy(temp, command);

    if(temp == NULL || !strcmp(temp,""))
    {
        listOfFiles(currpath, "", "","",0);
        listOfDir(currpath, "", "","");

        return NULL;
    }

    char *token = strtok(temp, " \t");

    char *prefix = (char *)malloc(MY_LEN * sizeof(char));

    if (strcmp(token, "cd") == 0)
    {
        if (debug)
            printf("cdflag = 1\n");
        strcpy(prefix, "cd");
        cdflag = 1;
    }
    else if (strcmp(token, "cat") == 0)
    {
        if (debug)
            printf("catflag = 1\n");
        strcpy(prefix, "cat");
        catflag = 1;
    }
    else if (strcmp(token, "ls") == 0)
    {
        if (debug)
            printf("lsflag = 1\n");
        strcpy(prefix, "ls");
        lsflag = 1;
    }
    else
    {
        printf("%c", 7);
        fflush(stdout);
        return command;
    }
    i++;

    char path[MY_LEN] = "";

    while (token != NULL)
    {
        if (debug)
        {
            printf("token in ac = :%s\n", token);
        }
        strcpy(path, token);
        token = strtok(NULL, " \t");
        i++;
    }

    if (i == 2)
    {
        printf("%c", 7);
        fflush(stdout);
        return command;
    }

    if (i > 3)
    {
        printf("Too many arguements \n %c", 7);
        fflush(stdout);
        return command;
    }

    // if(catflag)
    // {
    if (debug)
        printf("catflag vhjfvhjvjh\n");
    listOfFiles(currpath, path, prefix, command,1);
    listOfDir(currpath, path, prefix, command);
    // }

    return NULL;
}