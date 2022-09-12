#include "headers.h"
#include "functions.h"

void foregroundProcess(char *path, char *arguements[], int argc)
{
    if (debug)
        printf("running fg command : %s\n", arguements[0]);
    if (debug)
    {
        for (int i = 0; i < argc; i++)
        {
            printf("%d) %s\n", i, arguements[i]);
        }
    }

    int pid = fork();
    if (pid < 0)
    {
        perror("Fork error : ");
        return;
    }

    if (pid == 0)
    {
        if (execvp(arguements[0], arguements))
        {
            if(debug) printf("The process %s doesnt not exist \n",arguements[0]);
            perror("Foreground Process error ");
            exit(1);
        }

        exit(1);
        return ;
    }
    else
    {
        int wstatus;
        do
        {
            int w = waitpid(pid, &wstatus, WUNTRACED | WCONTINUED);
            if (w == -1)
            {
                perror("waitpid");
                exit(EXIT_FAILURE);
            }
            if (debug)
            {
                if (WIFEXITED(wstatus))
                {
                    if (pdebug)
                        printf("exited, status=%d\n", WEXITSTATUS(wstatus));
                }
                else if (WIFSIGNALED(wstatus))
                {
                    if (pdebug)
                        printf("killed by signal %d\n", WTERMSIG(wstatus));
                }
                else if (WIFSTOPPED(wstatus))
                {
                    if (pdebug)
                        printf("stopped by signal %d\n", WSTOPSIG(wstatus));
                }
                else if (WIFCONTINUED(wstatus))
                {
                    if (pdebug)
                        printf("continued\n");
                }
            }
        } while (!WIFEXITED(wstatus) && !WIFSIGNALED(wstatus));


                strcpy(bgp[bgpno], arguements[0]);
                if (debug)
                    printf("bgp[%d] = %s\n", bgpno, bgp[bgpno]);
                bgpid[bgpno] = pid;
                if (debug)
                    printf("bgpid[%d] = %d\n", bgpno, bgpid[bgpno]);
                exists[bgpno] = 0;
                if (debug)
                    printf("exists[%d] = %d\n", bgpno, exists[bgpno]);
                bgpno++;

    }
    return;
}

void backgroundProcess(char *path, char *arguements[], int argc)
{
    if (debug)
        printf("running bg command : %s\n", arguements[0]);
    if (debug)
    {
        for (int i = 0; i < argc; i++)
        {
            printf("%d) %s\n", i, arguements[i]);
        }
    }

    for (int i = 0; arguements[i] != NULL; i++)
    {
        if (!strcmp(arguements[i], "&"))
        {
            arguements[i] = NULL;
            break;
        }
    }

    if(arguements[0] == NULL) return;

    int pid = fork();
    if (pid < 0)   
    {
        perror("Fork error : ");
        return;
    }

    if (pid == 0)
    {
        setpgid(0, 0);
        if (execvp(arguements[0], arguements))
        {
            printf("Error: Command not Found: %s\n", arguements[0]);
            exit(1);
        }

        exit(1);
    }
    else
    {
        strcpy(bgp[bgpno], arguements[0]);
        if (debug)
            printf("bgp[%d] = %s\n", bgpno, bgp[bgpno]);
        bgpid[bgpno] = pid;
        if (debug)
            printf("bgpid[%d] = %d\n", bgpno, bgpid[bgpno]);
        exists[bgpno] = 1;
        if (debug)
            printf("exists[%d] = %d\n", bgpno, exists[bgpno]);
        bgpno++;

        printf("[%d] %d\n", bgpno, pid);
    }
    return;
}

void systemCommand(char *path, char *command)
{
    char *x = strtok(command, " \t");
    char *args[MY_LEN];
    int totalargs = 0;
    while (x != NULL)
    {
        args[totalargs] = (char *)malloc(MY_LEN * sizeof(char));
        strcpy(args[totalargs], x);
        totalargs++;
        x = strtok(NULL, " \t");
    }
    args[totalargs] = NULL;
    totalargs++;
    
    if(totalargs < 2) return;

    if (!strcmp(args[totalargs - 2], "&"))
    {
        backgroundProcess(path, args, totalargs);
    }
    else
    {

        struct timespec start, end;

        clock_gettime(CLOCK_REALTIME, &start);

        foregroundProcess(path, args, totalargs);

        clock_gettime(CLOCK_REALTIME, &end);

        int time_spent = (end.tv_sec - start.tv_sec) +
                            (end.tv_nsec - start.tv_nsec) / 1000000000.0;

        if (debug)
            printf("The elapsed time is %d seconds", time_spent);

        strcpy(extension, "");

        if (time_spent >= 1)
        {
            sprintf(extension, "%d", time_spent);
            strcat(extension, "s taken");
        }
    }
}


int LookforBG()
{
    int retval = 0;
    for (int i = 0; i < bgpno; i++)
    {
        // printf("%s %d %d\n",bgp[i],bgpid[i],exists[i]);
        if (exists[i])
        {
            retval = 1;
            char exit_status[MY_LEN] = "";
            int wstatus = 0;

            int w = waitpid(bgpid[i], &wstatus, WNOHANG);
            if (w > 0)
            {
                do
                {
                    if (w == -1)
                    {
                        perror("uwu waitpid");
                        // exit(EXIT_FAILURE);
                    }

                    if (WIFEXITED(wstatus))
                    {
                        if (pdebug)
                            printf("exited, status=%d\n", WEXITSTATUS(wstatus));
                        strcpy(exit_status, "exited normally");
                    }
                    else if (WIFSIGNALED(wstatus))
                    {
                        if (pdebug)
                            printf("killed by signal %d\n", WTERMSIG(wstatus));
                        strcpy(exit_status, "killed by signal");
                    }
                    else if (WIFSTOPPED(wstatus))
                    {
                        if (pdebug)
                            printf("stopped by signal %d\n", WSTOPSIG(wstatus));
                        strcpy(exit_status, "stopped by signal");
                    }
                    else if (WIFCONTINUED(wstatus))
                    {
                        if (pdebug)
                            printf("continued\n");
                        strcpy(exit_status, "continued");
                    }
                    else
                    {
                        if (pdebug)
                            printf("exited abnormally\n");
                        strcpy(exit_status, "exited abnormally");
                    }

                } while (!WIFEXITED(wstatus) && !WIFSIGNALED(wstatus));

                fprintf(stderr,"[%d]   Done--------%s [with pid = %d] was exceuted in background and %s\n", i+1, bgp[i], bgpid[i], exit_status);
                exists[i] = 0;
            }
        }
    }

    return retval;
}

void KillAllBG()
{
    for (int i = 0; i < bgpno; i++)
    {
        // printf("%s %d %d\n",bgp[i],bgpid[i],exists[i]);
        if (exists[i])
        {
            kill(bgpid[i], SIGKILL);
            printf("Killed %s [with pid = %d] \n", bgp[i], bgpid[i]);
        }
    }
}

char* AmpersandSpacer(char * command)
{
    if(debug) printf("cmd before spacing = %s\n",command);

    char* ret = (char *) malloc(MY_LEN* sizeof(char));
    int size = 0;
    for(int i = 0 ; i < strlen(command) ; i++)
    {
        if(command[i] == '&')
        {
            ret[size] = ' '; size++;
            ret[size] = '&'; size++;
            ret[size] = ' '; size++;
        }
        else
        {
            ret[size] = command[i]; size++;
        }
    }
    ret[size] = '\0';

    if(debug) printf("cmd after spacing = %s\n",ret);
    strcpy(command,ret);
    return ret;
}