#include "headers.h"
#include "functions.h"


char *executeCommand(char *command, char *path)
{
    if (debug)
        printf("EXECUTEcommand = %s\n", command);
    char entry[MY_LEN] = "";
    strcpy(entry, command);
    char copy[MY_LEN] = "";
    strcpy(copy,command);
    if(debug) printf("copy = %s\n",copy);
    char* NewCommand = HandleRedirection(copy);
    if(debug) printf("NewCommand after handling = %s\n",NewCommand);
    if(NewCommand == NULL) return NULL;
    strcpy(command, " ");
    strcpy(command, NewCommand);
    if (debug)
        printf("NEW EXECUTEcommand = %s\n", NewCommand);
    char temp[MY_LEN] = "";
    strcpy(temp,NewCommand);
    char *cmd = strtok(NewCommand," \t");
    if(debug) printf("temp = %s\n",temp);
    if (debug)
        printf("cmd = %s\n", cmd);
        
    if (cmd == NULL)
    {
        return NULL;
    }
    else if (!strcmp(cmd, "cd"))
    {
        cd(temp, path);
    }
    else if (!strcmp(cmd, "pwd"))
    {
        pwd();
    }
    else if (!strcmp(cmd, "echo"))
    {
        Echo(temp);
    }
    else if (!strcmp(cmd, "ls"))
    {
        ls(path, temp);
    }
    else if (!strcmp(cmd, "quit") || !strcmp(cmd,"exit"))
    {
        QUIT = 1;
    }
    else if (!strcmp(cmd, "pinfo"))
    {
        pinfo(temp);
    }
    else if (!strcmp(cmd, "discover"))
    {
        discover(path, temp);
    }
    else if (!strcmp(cmd, "history"))
    {
        // AddHistory(entry);
        // createHistory();
        history();
        return NULL;
    }
    else if(!strcmp(cmd, "jobs"))
    {
        jobs(temp);
    }
    else if(!strcmp(cmd, "sig"))
    {
        sig(temp);
    }
    else if(!strcmp(cmd, "fg"))
    {
        fg(temp);
    }
    else if(!strcmp(cmd, "bg"))
    {
        bg(temp);
    }
    else if (!strcmp(cmd, ""))
    {
        return NULL;
    }
    else
    {
        // printf("Error: %s: Command not found\n", cmd);
        systemCommand(path, temp);
    }

    if (debug)
        printf("execmd sends %s to his\n", temp);

    ResetStreams();
    // AddHistory(entry);
}
