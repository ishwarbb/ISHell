#include "headers.h"
#include "functions.h"

void controlZ(int signum) {
    printf("\n");
    char path[MY_LEN];
    getcwd(path,MY_LEN);
    prompt(path);
    return;
}

void controlC(int signum) {
    printf("\n");
    char path[MY_LEN];
    getcwd(path,MY_LEN);
    prompt(path);
    return;
    // exit(0);
}

void bindSignals()
{
    signal(SIGINT,controlC);
    signal(SIGTSTP,controlZ);
}

void sig(char* command)
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
        else if(args == 1)
        {
            sigNo = atoi(token);
            if(debug) perror("sigNo arguement\n");
            args++;
        }
        else
        {
            fprintf(stderr,"Too many arguements for sig\n");
            return;
        }

        token = strtok(NULL," \t");
    }

    if(args != 2)
    {
        fprintf(stderr,"Too few arguements for sig\n");
        return;
    }

    if(jobNo >= bgpno)
    {
        fprintf(stderr,"No such job exists \n");
    }
    kill(bgpid[jobNo],sigNo);

}
