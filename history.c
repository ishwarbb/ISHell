#include "headers.h"
#include "functions.h"

void createHistory()
{
    strcpy(hispath, HomeDirectory);
    strcat(hispath, "/.ish_history");

    if (debug)
        printf("about to create history\n");
    FILE *temp = fopen(hispath, "w");
    if (debug)
        if (temp == NULL)
        {
            printf("could not create history\n");
            return;
        }
    fprintf(temp, "0 \n");
    fclose(temp);
    if (debug)
        perror("while creating history : ");
    if (debug)
        printf("c h failed\n");
}

void history()
{
    strcpy(hispath, HomeDirectory);
    strcat(hispath, "/.ish_history");

    FILE *f = fopen(hispath, "r");
    if (f == NULL)
    {
        createHistory();
    }
    else
    {
        fclose(f);
    }

    f = fopen(hispath, "r");
    int size;
    // fscanf(f, "%d", &size);
    char buf[MY_LEN] = "";
    fgets(buf, MY_LEN, f);
    size = atoi(buf);

    char *commands[size + 1];
    if (debug)
    {
        printf("commands array created\n");
    }

    for (int i = 0; i <= size; i++)
    {
        commands[i] = (char *)malloc(MY_LEN * sizeof(char));
    }

    for (int i = 0; i < size; i++)
    {
        // fscanf(f, "%s", commands[i]);
        fgets(commands[i], MY_LEN, f);
        if (debug)
            printf("scanning commands[%d] = %s\n", i, commands[i]);
        // DeDashify(commands[i]);
    }

    for (int i = max(size - 10 ,0); i < size; i++)
    {
        printf("%s", commands[i]);
        fputs(commands[i], f);
        if (debug)
            printf("printing commands[%d] = %s\n", i, commands[i]);
    }
    fclose(f);
}

void AddHistory(char *command)
{
    // Dashify(command);
    char entry[MY_LEN];
    strcpy(entry, command);
    strcat(entry, "\n");

    if (debug)
        printf("\nadding to history %s\n", entry);

    strcpy(hispath, HomeDirectory);
    strcat(hispath, "/.ish_history");

    int createhistory = 0;

    FILE *f = fopen(hispath, "r");
    if (f == NULL)
    {
        createhistory = 1;
    }
    else
    {
        fclose(f);
    }

    if (debug)
        printf("calling createHistory()\n");
    if (createhistory)
        createHistory();

    f = fopen(hispath, "r");
    if (debug)
    {
        printf("hispath = %s\n",hispath);
        // perror("opening if created");
    }
    int size;
    // fscanf(f, "%d", &size);
    char buf[MY_LEN] = "";
    fgets(buf, MY_LEN, f);
    size = atoi(buf);
    if (debug)
        printf("size read = %d\n", size);

    char *commands[size + 1];
    if (debug)
    {
        printf("commands array created\n");
    }

    for (int i = 0; i <= size; i++)
    {
        commands[i] = (char *)malloc(MY_LEN * sizeof(char));
    }

    for (int i = 0; i < size; i++)
    {
        if (debug)
            printf("commands[%d] getting it \n", i);
        // fscanf(f, "%[^\n]%*c", commands[i]);
        fgets(commands[i], MY_LEN, f);
        if (debug)
            printf("commands[%d] = %s\n", i, commands[i]);
    }
    fclose(f);

    if (size > 0)
    {
        if (debug)
            printf("command = %s\n", command); // USE ENTRY HERE
        if (debug)
            printf("commands[size] = %s\n", commands[size - 1]);
        if (!strcmp(entry, commands[size - 1]))
        {
            return;
        }
    }

    f = fopen(hispath, "w");
    strcpy(commands[size], entry);
    if (debug)
        printf("after copying = %s\n", commands[size]);
    if ((size + 1) > 20)
    {
        assert(size == 20);
        fprintf(f, "%d \n", 20);
        for (int i = 1; i < 21; i++)
        {
            // fprintf(f, "%s ", commands[i]);
            fputs(commands[i], f);
        }
    }
    else
    {
        fprintf(f, "%d \n", size + 1);
        for (int i = 0; i < (size + 1); i++)
        {
            // fprintf(f, "%s ", commands[i]);
            fputs(commands[i], f);
        }
    }
    fclose(f);
}