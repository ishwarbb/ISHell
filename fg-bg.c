#include "headers.h"

void fg(char* command)
{
   char* token = strtok(command," \t");
    if(strcmp(token,"sig") != 0)
    {
        if(debug) printf("the cmd is not sig \n");
    }

    int jobNo;
    int sigNo;
    
    token = strtok(NULL," \t");
    int args = 0;
    while(token != NULL)
    {
        if(args == 0)
        {
            jobNo = atoi(token) - 1;
            if(debug) perror("jobNo arguement\n");
            args++;
        }
        else
        {
            fprintf(stderr,"Too many arguements for sig\n");
            return;
        }

        token = strtok(NULL," \t");
    }

    if(args != 1)
    {
        fprintf(stderr,"Too few arguements for sig\n");
        return;
    }

    if(jobNo >= bgpno)
    {
        fprintf(stderr,"No such job exists \n");
    }
}

void bg(char* command)
{
   char* token = strtok(command," \t");
    if(strcmp(token,"sig") != 0)
    {
        if(debug) printf("the cmd is not sig \n");
    }

    int jobNo;
    int sigNo;
    
    token = strtok(NULL," \t");
    int args = 0;
    while(token != NULL)
    {
        if(args == 0)
        {
            jobNo = atoi(token) - 1;
            if(debug) perror("jobNo arguement\n");
            args++;
        }
        else
        {
            fprintf(stderr,"Too many arguements for sig\n");
            return;
        }

        token = strtok(NULL," \t");
    }

    if(args != 1)
    {
        fprintf(stderr,"Too few arguements for sig\n");
        return;
    }

    if(jobNo >= bgpno)
    {
        fprintf(stderr,"No such job exists \n");
    }
}