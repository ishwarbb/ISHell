#include "headers.h"
#include "functions.h"


void jobs(char* command)
{
    int jobs_r_flag = 0;
    int jobs_s_flag = 0;

    char * token = strtok(command," \t");
    if(strcmp(token,"jobs") != 0)
    {
        if(debug) printf("Not a jobs command, Wrong number\n");
    }

    token = strtok(NULL," \t");

    while(token != NULL)
    {
        if(!strcmp(token,"-r"))
        {
            jobs_r_flag = 1;
        }
        else if (!strcmp(token,"-s"))
        {
            jobs_s_flag = 1; 
        }
        else
        {
            fprintf(stderr,"Too many arguements for jobs\n");
        }
    }

    if((jobs_r_flag == 0) && (jobs_s_flag == 0))
    {
        jobs_r_flag = 1;
        jobs_s_flag = 1;
    }

    for(int i =0 ; i < bgpno; i++)
    {
        if(exists[i]) 
        {
            printf("[%d] Running %s [%d]\n",i,bgp[i],bgpid[i]);
        }
        else
        {
            printf("[%d] Stopped %s [%d]\n",i,bgp[i],bgpid[i]);
        }
    }

}