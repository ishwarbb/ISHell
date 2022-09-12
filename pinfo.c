#include "headers.h"
#include "functions.h"

void pinfo(char *command)
{
    if (debug)
        printf("pinfo commmand = %s\n", command);
    char *token = strtok(command, " \t");
    if (debug)
        printf("no more tok after this one - %s\n", token);
    if (strcmp(token, "pinfo") != 0)
    {
        printf("this is not a pinfo command \n");
    }

    int pid = getpid();
    char pidstring[MY_LEN];
    sprintf(pidstring, "%d", pid);

    token = strtok(NULL, " \t");
    if (token != NULL)
    {
        strcpy(pidstring, token);
    }
    token = strtok(NULL, " \t");
    if (token != NULL)
        printf("Too many arguments for pinfo\n");

    char pinfoPath[MY_LEN];
    strcpy(pinfoPath, "/proc/");
    strcat(pinfoPath, pidstring);
    strcat(pinfoPath, "/stat");

    FILE *fp = fopen(pinfoPath, "r");
    if (fp == NULL)
    {
        printf("Invalid PID given: The path %s doesnt exist\n", pinfoPath);
        return;
    }

    int processGroupId;

    char temp[MY_LEN];
    fscanf(fp, "%s", temp);
    fscanf(fp, "%s", temp);
    fscanf(fp, "%s", temp);

    char processStatus[MY_LEN];
    strcpy(processStatus, temp);

    fscanf(fp, "%s", temp);
    fscanf(fp, "%s", temp);

    processGroupId = atoi(temp);

    fclose(fp);

    if (processGroupId == tcgetpgrp(0))
        strcat(processStatus, "+");

    strcpy(pinfoPath, "/proc/");
    strcat(pinfoPath, pidstring);
    strcat(pinfoPath, "/stat");

    fp = fopen(pinfoPath, "r");
    if (fp == NULL)
    {
        printf("Invalid PID given: The path %s doesnt exist\n", pinfoPath);
        return;
    }
    for(int i = 0; i < 23 ;i++)
    {fscanf(fp, "%s", temp);}
    fclose(fp);

    char processMemory[MY_LEN];
    strcpy(processMemory, temp);

    strcpy(pinfoPath, "/proc/");
    strcat(pinfoPath, pidstring);
    strcat(pinfoPath, "/exe");

    fp = fopen(pinfoPath, "r");
    if (fp == NULL)
    {
        printf("Invalid PID given: The path %s doesnt exist\n", pinfoPath);
        return;
    }

    fscanf(fp, "%s", temp);

    char processExe[MY_LEN];
    strcpy(processExe, temp);

    int x = readlink(pinfoPath, processExe, MY_LEN);
    if (x == -1)
        strcpy(processExe, "Does not exist");

    printf("pid : %s\n", pidstring);
    printf("prosess status : %s\n", processStatus);
    printf("memory : %s {Virtual Memory}\n", processMemory);
    printf("executable path : %s\n", Tildify(processExe));
}