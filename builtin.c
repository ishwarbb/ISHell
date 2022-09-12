#include "headers.h"
#include "functions.h"

void pwd()
{
    char buff[MY_LEN];
    char *ret = getcwd(buff, MY_LEN);
    // printf("We are at : %s\n", buff);
    printf("%s\n", buff);
    if (debug)
    {
        printf("getcwd returned %s\n", ret);
        perror("getcwd ");
    }
}

char *cd(char *command, char *path)
{
    char currpath[MY_LEN];
    strcpy(currpath, path);

    char extrapath[MY_LEN];
    if (debug)
        printf("CD command = %s\n", command);
    char *token = strtok(command, " \t");
    if (debug)
        printf("no more tok after this one - %s\n", token);
    if (!strcpy(token, "cd"))
    {
        printf("this is not a cd command error \n");
    }
    token = strtok(NULL, " \t");

    if (token == NULL)
    {
        if (debug)
            printf("token is null\n");
        token = HomeDirectory;
    }

    char *end = strtok(NULL, " \t");
    if (end != NULL)
    {
        printf("Too many arg for cd\n");
        return NULL;
    }

    DeTildify(token);

    if (!strcmp(token, "-"))
    {
        if (!strcmp(PreviousDirectory, "uwu"))
        {
            printf("Error: Old pwd not set\n");
            return NULL;
        }
        strcpy(token, PreviousDirectory);
        printf("%s\n", PreviousDirectory);
        if (debug)
        {
            printf("Previous directory = %s\n", PreviousDirectory);
        }
    }

    if (debug)
        printf("CD token = %s\n", token);

    char cwd[256];

    if (chdir(token) != 0)
    {
        // if (debug)
        // {
        perror("cd error()");
        printf("cannot change path to %s\n", token);
        // }
    }
    else
    {
        if (getcwd(cwd, sizeof(cwd)) == NULL)
        {
            // if (debug)
            // {
            perror("getcwd() error");
            // }
        }
        else
        {
            if (debug)
            {
                printf("current working directory is: %s\n", cwd);
            }
            strcpy(PreviousDirectory, path);
            strcpy(path, cwd);
        }
    }
}

// void Echo(char* temp)
// {
//     int flag1 = 0;
//     int flag2 = 0;
//     int flag3 = 0;
//     int flag4 = 0;
//     int flag = 0;
//
//     int start;
//
//     for(int i = 0; i < strlen(temp); i++)
//     {
//         if(flag == 0)
//         {
//             if(temp[i] == 'e') flag1 = 1;
//             if(temp[i] == 'c') flag2 = 1;
//             if(temp[i] == 'h') flag3 = 1;
//             if(temp[i] == 'o') flag4 = 1;
//             flag = flag1 & flag2 & flag3 & flag4;
//         }
//         else
//         {
//             start = i;
//             break;
//         }
//     }
//
//     for(int i = start; i < strlen(temp); i++)
//     {
//         if(temp[i] == ' ') {}
//         else
//         {
//             start = i;
//             break;
//         }
//     }
//
//     for(int i = start; i < strlen(temp); i++)
//     {
//         printf("%c",temp[i]);
//     }
//     printf("\n");
// }

void Echo(char *command)
{
    char *Words[MY_LEN];
    for (int i = 0; i < MY_LEN; i++)
    {
        Words[i] = malloc(MY_LEN * sizeof(char));
    }
    char *token = strtok(command, " \t");
    int totalwords = 0;
    while (token != NULL)
    {
        if (debug)
            printf("echo token = %s\n", token);
        strcpy(Words[totalwords], token);
        totalwords++;
        token = strtok(NULL, " \t");
    }

    for (int i = 1; i < totalwords; i++)
    {
        printf("%s ", Words[i]);
    }
    printf("\n");
}