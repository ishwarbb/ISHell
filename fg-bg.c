#include "headers.h"

void fg(char* command)
{
    char* token = strtok(command," \t");
    if(strcmp(token,"bg") != 0)
    {
        if(debug) printf("the cmd is not bg \n");
    }
    
    token = strtok(NULL," \t");
    int args = 0;

    char jid[MY_LEN];
    while(token != NULL)
    {
        if(args == 0)
        {
            if(debug) perror("jobid arguement\n");
            args++;
            strcpy(jid,token);
        }
        else
        {
            fprintf(stderr,"Too many arguements for bg\n");
            return;
        }

        token = strtok(NULL," \t");
    }

    if (args == 0)
    {
        fprintf(stderr, "Error: enter valid jobIndex\n");
        return;
    }

    int jobIndex = atoi(jid);

    if (jobIndex > bgpno)
    {
        fprintf(stderr, "No such process\n");
        return;
    }

    // Getting process information from child pool
    pid_t pid = bgpid[jobIndex - 1];
    char pName[MY_LEN];
    strcpy(pName, bgp[jobIndex - 1]);

    // // Removing process from child pool because it is being moved to the foreground
    // removeChild(pid);
    bexists[jobIndex - 1] = 0;

    // // Ignoring required signals from the shell
    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);

    // // Setting foreground process flag to 1 to indicate that there is a foreground process running
    int fgP = 1;

    // // Giving the specified process terminal control and checking for errors
    if(tcsetpgrp(STDIN_FILENO, getpgid(pid)))
    {
        fprintf(stderr, "Could not give terminal control to job %d\n", jobIndex);
        // Control couldn't be give to the child process, we want the shell to continue
        // Set signal handlers to default
        signal(SIGTTOU, SIG_DFL);
        signal(SIGTTIN, SIG_DFL);
        // Print error message and return from the function.
        return ;
    }
    // // Telling the job to resume and handling errors (same as above)
    if(kill(pid, SIGCONT))
    {
        fprintf(stderr, "Unable to resume job %d\n", jobIndex);
        signal(SIGTTOU, SIG_DFL);
        signal(SIGTTIN, SIG_DFL);
        return ;
    }

    // // Waiting for foreground process to be stopped or to terminate.
    // // If it is stopped (Ctrl+Z), add it to the child pool
    int status;
    if(waitpid(pid, &status, WUNTRACED) > 0)
        if(WIFSTOPPED(status))
        {
            strcpy(bgp[bgpno], pName);
            if (debug)
                printf("bgp[%d] = %s\n", bgpno, bgp[bgpno]);
            bgpid[bgpno] = pid;
            if (debug)
                printf("bgpid[%d] = %d\n", bgpno, bgpid[bgpno]);
            bexists[bgpno] = 1;
            if (debug)
                printf("bexists[%d] = %d\n", bgpno, bexists[bgpno]);
            bgpno++;
        }
    
    // exitCode = WEXITSTATUS(status) ? -1 : 1;

    // // Since the foreground process has been exitted from, set the flag back to 0
    // fgP = 0;
    // // Return terminal control back to the shell and perform error handling
    // // If control cannot be returned, quit the program.
    if(tcsetpgrp(STDIN_FILENO, getpgid(0)))
    {
        fprintf(stderr, "Could not return terminal controll to the shell. Exitting the shell\n");
        return;
    }
    // // Restore default signal handlers
    signal(SIGTTOU, SIG_DFL);
    signal(SIGTTIN, SIG_DFL);
    return;
}

void bg(char* command)
{
    char* token = strtok(command," \t");
    if(strcmp(token,"bg") != 0)
    {
        if(debug) printf("the cmd is not bg \n");
    }
    
    token = strtok(NULL," \t");
    int args = 0;

    char jid[MY_LEN];
    while(token != NULL)
    {
        if(args == 0)
        {
            if(debug) perror("jobid arguement\n");
            args++;
            strcpy(jid,token);
        }
        else
        {
            fprintf(stderr,"Too many arguements for bg\n");
            return;
        }

        token = strtok(NULL," \t");
    }

    if (args == 0)
    {
        fprintf(stderr, "Error: enter valid jobIndex\n");
        return;
    }

    int jobIndex = atoi(jid) - 1;

    if (jobIndex > bgpno)
    {
        fprintf(stderr, "No such process\n");
        return;
    }

    kill(bgpid[jobIndex], SIGCONT);

    return;
}