#include "headers.h"
#include "functions.h"

char HomeDirectory[MY_LEN];
char PreviousDirectory[MY_LEN];
char hispath[MY_LEN];
char extension[MY_LEN];
int QUIT;
int errno;
char *bgp[MY_LEN];
int bgpid[MY_LEN];
int exists[MY_LEN];
int bgpno;
int initSTDIN;
int initSTDOUT;
int fd1;
int fd2;
int redirread;
int redirwrite;

int main()
{
    init();
    bindSignals();

    struct utsname unameBuffer;

    dup2(0, STDIN_FILENO);
    dup2(1, STDOUT_FILENO);

    char buff[MY_LEN];
    char *ret = getcwd(buff, MY_LEN);
    strcpy(HomeDirectory, buff);

    errno = 0;
    if (uname(&unameBuffer) < 0)
    {
        perror("uname");
        exit(EXIT_FAILURE);
    }
    
    promptInit();

    char path[MY_LEN];
    strcpy(path, HomeDirectory);

    for (int i = 0; i < MY_LEN; i++)
    {
        bgp[i] = (char *)malloc(MY_LEN * sizeof(char));
    }

    prompt(path);
    while (1)
    {
        char command[MY_LEN];
        char *inputstatus = fgets(command, MY_LEN, stdin);
        if (inputstatus == NULL)
        {
            printf("\nEOF reached in input Stream. I QUIT!\n");
            return 0;
        }
        command[strlen(command) - 1] = '\0';
        AddHistory(command);
        strcat(command, ";");
        if (debug)
            printf("command = %s\n", command);

        char *Commands[MY_LEN];
        for (int i = 0; i < MY_LEN; i++)
        {
            Commands[i] = malloc(MY_LEN * sizeof(char));
        }
        char *token = strtok(command, ";");
        int totalCommands = 0;
        while (token != NULL)
        {
            if (debug)
                printf("token = %s\n", token);
            // executeCommand(token, path);
            strcpy(Commands[totalCommands], token);
            totalCommands++;
            if (debug)
                printf("hellp\n");
            token = strtok(NULL, ";");
        }

        if (debug)
            printf("total commands = %d", totalCommands);

        for (int k = 0; k < totalCommands; k++)
        {
            char **x = AmpersandHandler(Commands[k]);
            if (debug)
                printf("a1\n");
            for (int i = 0;; i++)
            {
                if (x[i] == NULL)
                    break;
                if (debug)
                    printf("ai = %d\n", i);
                if (debug)
                    printf("bg ps in main are %s\n", x[i]);
                executeCommand(x[i], DeTildify(path));
            }
            if (debug)
                printf("uwu\n");
            // executeCommand(Commands[k], DeTildify(path));
        }

        LookforBG();

        if (QUIT)
        {
            for (int i = 0; i < MY_LEN; i++)
            {
                free(Commands[i]);
            }

            if (LookforBG())
            {
                printf("There are some processes still running.\n");
                printf("Do you still wanna quit? [Y/n]:");
                char c;
                scanf("%c", &c);
                if (c == 'y' || c == 'Y')
                {
                    KillAllBG();
                    printf("All running processes killed\n");
                }
                else
                {
                    printf("quit declined\n");
                }

                return 0;
            }
            else
            {
                return 0;
            }
        }

        // printf("\033[1;36m<%s@%s:%s\033[0m\033[1;33m%s%s>\033[0m $ ", username, unameBuffer.sysname, "", Tildify(path), extension);
        // fflush(stdout);
        prompt(path);

        strcpy(extension, "");
    }
}