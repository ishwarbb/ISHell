#include "headers.h"
#include "functions.h"

int max(int a, int b)
{
    if(a > b) return a;
    return b;
}

void PrintStringArray(char** array, int n)
{
    printf("Array: ");
    for (int i = 0; i < n; i++)
        printf("%s \n", array[i]);
    printf("\n");
}

void PrintNumArray(char *array, int n)
{
    printf("Array: ");
    for (int i = 0; i < n; i++)
        printf("%d \n", array[i]);
    printf("\n");
}

int HasQuotes(char *word)
{
    int l = strlen(word);
    if (word[0] == '"' && word[l - 1] == '"')
    {
        if (debug)
            printf("word %s has quotes \n", word);
        return 1;
    }
    else
    {
        if (debug)
            printf("word %s does not have quotes \n", word);
        return 0;
    }
}

char *removeQuotes(char *word)
{
    if (!HasQuotes(word))
    {
        if (debug)
            printf("No need to remove quotes\n");
        return word;
    }

    char temp[MY_LEN] = "";
    strncpy(temp, &word[1], strlen(word) - 2);
    if (debug)
        printf("temp = %s\n", temp);
    strcpy(word, temp);
    strcat(word, "\0");

    if (debug)
        printf("word after removing quotes %s \n", word);

    return word;
}

char *Dotify(char *path)
{
    char cwd[MY_LEN];
    char *ret = getcwd(cwd, MY_LEN);

    if (debug)
        printf("cwd for dotify = %s\n", cwd);

    if (strncmp(path, cwd, strlen(cwd)) != 0)
    {
        if (debug)
            printf("No need to dotifyy\n");
        return path;
    }

    if (path[strlen(HomeDirectory)] != '\0' && path[strlen(HomeDirectory)] != '/')
    {
        if (debug)
            printf("No need to dotify\n");
        return path;
    }

    char *newpath = (char *)malloc(MY_LEN * sizeof(char));
    strcpy(newpath, ".");
    char extra[MY_LEN];
    if (debug)
    {
        printf("strlen HD = %ld\n", strlen(cwd));
    }
    strcpy(extra, &path[strlen(cwd)]);
    if (debug)
        printf("extra sfter dot = %s\n", extra);
    strcat(newpath, extra);
    if (debug)
        printf("newdotpath = %s\n", newpath);

    return newpath;
}

char *DeDotify(char *path)
{
    char cwd[MY_LEN];
    char *ret = getcwd(cwd, MY_LEN);

    if (debug)
        printf("dedotify path[0] is %c\n", path[0]);
    if (path[0] == '.')
    {
        char *t = (char *)malloc(MY_LEN * sizeof(char));
        strcpy(t, cwd);
        strcat(t, &path[1]);
        strcpy(path, t);
    }
    if (debug)
        printf("after dotifying = %s\n", path);
    return path;
}

char *AbsPathofDir(char *leading, char *pwd)
{
    FILE *f;
    char prevpath[MY_LEN];
    getcwd(prevpath, MY_LEN);
    chdir(leading);

    DIR *x = opendir(leading);
    if (x == NULL)
    {
        chdir(prevpath);
        if (debug)
            perror("CheckValid dir failed");
        closedir(x);
        return 0;
    }
    closedir(x);

    char abs[MY_LEN] = "";
    getcwd(abs, MY_LEN);

    chdir(prevpath);
    if (debug)
    {
        printf("dir exists\n");
    }

    strcpy(leading, abs);
    return leading;
}

char *Dashify(char *command)
{
    if (debug)
        printf("dashifying %s\n", command);
    int l = strlen(command);
    for (int i = 0; i < l; i++)
    {
        if (command[i] == ' ')
        {
            command[i] = '_';
        }
        if (command[i] == '\t')
        {
            command[i] = '|';
        }
    }
}

char *DeDashify(char *command)
{
    int l = strlen(command);
    for (int i = 0; i < l; i++)
    {
        if (command[i] == '_')
        {
            command[i] = ' ';
        }
        if (command[i] == '|')
        {
            command[i] = '\t';
        }
    }
}

int CheckValidDir(char *dir, char *path)
{
    DIR *x = opendir(dir);
    if (x == NULL)
    {
        if (debug)
            perror("CheckValid dir failed");
        return 0;
    }
    if (debug)
        printf("dir exists\n");
    closedir(x);
    return 1;
}

int CheckValidFile(char *file, char *path)
{
    FILE *f;
    char prevpath[MY_LEN];
    getcwd(prevpath, MY_LEN);
    int x;
    chdir(path);
    if ((f = fopen(file, "r")))
    {
        fclose(f);
        x = 1;
        if (debug)
            printf("file exists\n");
    }
    else
    {
        x = 0;
        if (debug)
            printf("file doesn't exist\n");
    }
    chdir(prevpath);
    return x;
}

char *Tildify(char *path)
{
    if (strncmp(path, HomeDirectory, strlen(HomeDirectory)) != 0)
    {
        if (debug)
            printf("No need to tildify\n");
        return path;
    }

    if (path[strlen(HomeDirectory)] != '\0' && path[strlen(HomeDirectory)] != '/')
    {
        if (debug)
            printf("No need to tildify\n");
        return path;
    }

    char *newpath = (char *)malloc(MY_LEN * sizeof(char));
    strcpy(newpath, "~");
    char extra[MY_LEN];
    if (debug)
    {
        printf("strlen HD = %ld\n", strlen(HomeDirectory));
        printf("uuwu %s\n", &path[strlen(HomeDirectory)]);
    }
    strcpy(extra, &path[strlen(HomeDirectory)]);
    if (debug)
        printf("extra = %s\n", extra);
    strcat(newpath, extra);
    if (debug)
        printf("newpath = %s\n", newpath);

    return newpath;
}

char *DeTildify(char *path)
{
    if (debug)
        printf("detildify path[0] is %c\n", path[0]);
    if (path[0] == '~')
    {
        char *t = (char *)malloc(MY_LEN * sizeof(char));
        strcpy(t, HomeDirectory);
        strcat(t, &path[1]);
        strcpy(path, t);
    }
    return path;
}

char **AmpersandHandler(char *command)
{
    AmpersandSpacer(command);

    char** commands = (char**) malloc(MY_LEN*sizeof(char *));
    int size = 0;
    int totalAmpersands = 0;

    for(int i = 0; i < MY_LEN; i++)
    {
        commands[i] = NULL;
    }

    for (int i = 0; i < strlen(command); i++)
    {
        if (command[i] == '&')
            totalAmpersands++;
    }

    char *token = strtok(command, "&");
    while (token != NULL)
    {
        commands[size] = (char *)malloc(MY_LEN * sizeof(char));
        strcpy(commands[size], token);
        size++;
        token = strtok(NULL, "&");
    }

    // commands[totalAmpersands] = NULL;

    for (int i = 0; i < totalAmpersands; i++)
    {
        if(commands[i] == NULL) strcpy(commands[i],"  ");
        strcat(commands[i], " &");
        if(debug) printf("bg ps are %s\n", commands[i]);
    }

    if(debug) printf("Returning from ampersand handler\n");
    return commands;
}

