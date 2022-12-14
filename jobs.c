#include "headers.h"
#include "functions.h"

void jobs(char *command)
{
    int jobs_r_flag = 0;
    int jobs_s_flag = 0;

    char *token = strtok(command, " \t");
    if (strcmp(token, "jobs") != 0)
    {
        if (debug)
            printf("Not a jobs command, Wrong number\n");
    }

    token = strtok(NULL, " \t");

    while (token != NULL)
    {
        if (!strcmp(token, "-r"))
        {
            jobs_r_flag = 1;
        }
        else if (!strcmp(token, "-s"))
        {
            jobs_s_flag = 1;
        }
        else
        {
            fprintf(stderr, "Too many arguements for jobs\n");
            return;
        }
    }

    if ((jobs_r_flag == 0) && (jobs_s_flag == 0))
    {
        jobs_r_flag = 1;
        jobs_s_flag = 1;
    }

	char filePath[MY_LEN];
    char statRead[MY_LEN];

    // for(int i =0 ; i < bgpno; i++)
    // {
    //     if(exists[i])
    //     {
    //         printf("[%d] Running %s [%d]\n",i + 1,bgp[i],bgpid[i]);
    //     }
    //     else
    //     {
    //         printf("[%d] Stopped %s [%d]\n",i + 1,bgp[i],bgpid[i]);
    //     }
    // }

	for (int i = 0; i < bgpno; i++)
    {
        char pidstring[MY_LEN];
        sprintf(pidstring, "%d", bgpid[i]);

        if(bexists[i] == 0) continue;

        char pinfoPath[MY_LEN];
        strcpy(pinfoPath, "/proc/");
        strcat(pinfoPath, pidstring);
        strcat(pinfoPath, "/stat");

        FILE* fp = fopen(pinfoPath, "r");
        if (fp == NULL)
        {
            fprintf(stderr,"pid proc file doesnt exist\n");
            return;
        }

        char temp[MY_LEN];
        fscanf(fp, "%s", temp);
        fscanf(fp, "%s", temp);
        fscanf(fp, "%s", temp);

        char processStatus[MY_LEN];
        strcpy(processStatus, temp);

        printf("[%d] %s %s [%d]\n", i + 1, !strcmp(processStatus,"S") ? "Running" : "Stopped", bgp[i], bgpid[i]);
    }
}