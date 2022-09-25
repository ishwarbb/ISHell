#include "headers.h"
#include "functions.h"

void controlZ(int signum) {
    printf("\n");
    char path[MY_LEN];
    getcwd(path,MY_LEN);
    // prompt(path);
    return;
}

void controlC(int signum) {
    printf("\n");
    char path[MY_LEN];
    getcwd(path,MY_LEN);
    // prompt(path);
    return;
    // exit(0);
}


void bindSignals()
{
    signal(SIGINT,controlC);
    signal(SIGTSTP,controlZ);

    // struct sigaction sigint_action;
    // memset(&sigint_action, 0, sizeof(sigint_action));
    // sigint_action.sa_handler = controlC;
    // sigint_action.sa_flags = SA_RESTART;
    // sigemptyset(&sigint_action.sa_mask);
    // sigaction(SIGINT, &sigint_action, NULL);

    // struct sigaction sigtstp_action;
    // memset(&sigtstp_action, 0, sizeof(sigtstp_action));
    // sigtstp_action.sa_handler = controlZ;
    // sigtstp_action.sa_flags = SA_RESTART;
    // sigemptyset(&sigtstp_action.sa_mask);
    // sigaction(SIGTSTP, &sigtstp_action, NULL);
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
