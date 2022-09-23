#include "headers.h"
#include "functions.h"

void A(char *command, char *path)
{
    char **x = AmpersandHandler(command);
    if (debug)
        printf("a1\n");
    for (int i = 0;; i++)
    {
        if (x[i] == NULL)
            break;
        if (debug)
            printf("ai = %d\n", i);
        if (debug)
            printf("in A, x[i] = %s\n", x[i]);
        executeCommand(x[i], DeTildify(path));
    }
}

char **pipeLine(char *command, char *path)
{
    int exitcode = 1;

    int pipes = 1;
    int l = strlen(command);
    for (int i = 0; i < l; i++)
        if (command[i] == '|')
            pipes++;

    if (pipes == 1)
    {
        A(command, path);
        return NULL;
    }

    char **y = (char **)malloc(MY_LEN * sizeof(char *));
    for (int i = 0; i < MY_LEN; i++)
    {
        y[i] = (char*) malloc(MY_LEN* sizeof(char));
        strcpy(y[i],"");
    }

    int yindex = 0;
    y[0] = strtok(command, "|");
    while (y[yindex] != NULL)
    {
        yindex++;
        y[yindex] = strtok(NULL, "|");
    }

    int totalpipefiles = 2 * (yindex - 1);
    int pipefiles[totalpipefiles];

    for (int i = 0; i < totalpipefiles; i += 2)
    {
        int s = pipe(pipefiles + i);
        if (s == -1)
        {
            fprintf(stderr, "Unable to create pipe at %d\n", i);
            exitcode = -1;
            return NULL;
        }
    }

    for (int i = 0; i < yindex; i++)
    {
        int pid = fork();
        if (pid == 0)
        {
            if (i < (yindex - 1))
                dup2(pipefiles[2 * i + 1], STDOUT_FILENO);
            if (i > 0)
                dup2(pipefiles[2 * i - 2], STDIN_FILENO);
            for (int j = 0; j < totalpipefiles; j++)
                close(pipefiles[j]);
            A(y[i], path);
            exit(exitcode);
        }
    }
    for (int i = 0; i < totalpipefiles; i++)
        close(pipefiles[i]);
    // Reap all the dead child processes
    for (int i = 0; i < yindex; i++)
    {
        if (i == yindex - 1)
        {
            wait(&exitcode);
            exitcode = WEXITSTATUS(exitcode);
        }
        else
            wait(NULL);
    }
}

// typedef struct file
// {
//     int* fd;
// }file;


// char **pipeLine(char *command, char *path)
// {
//     char **y = (char **)malloc(MY_LEN * sizeof(char *));
//     for (int i = 0; i < MY_LEN; i++)
//     {
//         y[i] = NULL;
//     }

//     int exitcode = 1;

//     int pipes = 1;
//     for (int i = 0; i < strlen(command); i++)
//         if (command[i] == '|')
//             pipes++;

//     if (pipes == 1)
//     {
//         A(command, path);
//         return NULL;
//     }

//     int yindex = 0;
//     y[0] = strtok(command, "|");
//     while (y[yindex] != NULL)
//     {
//         yindex++;
//         y[yindex] = strtok(NULL, "|");
//     }

//     int totalcommands = yindex - 1;
    
//     file** pipefiles = (file**) malloc(totalcommands*sizeof(file*));
//     for(int i =0 ; i < totalcommands; i++)
//     {
//         pipefiles[i]->fd = (int*) malloc(2*sizeof(int));
//     }

//     for (int i = 0; i < totalcommands; i ++)
//     {
//         file* temp = pipefiles[i];
//         int s = pipe(temp->fd);
//         if (s == -1)
//         {
//             fprintf(stderr, "Unable to create pipe at %d\n", i);
//             return NULL;
//         }
//     }

//     for (int i = 0; i < totalcommands; i++)
//     {
//         int pid = fork();
//         if (pid == 0)
//         {
//             if(i != (totalcommands-1)) dup2(pipefiles[i]->fd[1], STDOUT_FILENO);
//             if(i != 0) dup2(pipefiles[i-1]->fd[0], STDIN_FILENO);
//             for (int j = 0; j < totalcommands; j++)
//             {
//                 close(pipefiles[j]->fd[0]);
//                 close(pipefiles[i]->fd[0]);
//             }
//             A(y[i], path);
//             exit(exitcode);
//         }
//     }

//     for (int i = 0; i < totalcommands; i++)
//     {
//         close(pipefiles[i]->fd[0]);
//         close(pipefiles[i]->fd[0]);
//     }

// }

// char** pipeLine(char *inputString, char* path)
// {
//     // Count the number of pipe separated commands
//     int comCount = 1;
//     int exitCode = 1;
//     int len = strlen(inputString);
//     for (int i = 0; i < len; i++)
//         if (inputString[i] == '|')
//             comCount++;

//     // If there is just one, no pipes, just run
//     if (comCount == 1)
//         executeCommand(inputString,path);

//     // Otherwise
//     else
//     {
//         // Assign one pointer to each command
//         char *commands[comCount];
//         for (int i = 0; i <= comCount; i++)
//             commands[i] = NULL;

//         comCount = 0;
//         commands[0] = strtok(inputString, "|");
//         while (commands[comCount] != NULL)
//         {
//             comCount++;
//             commands[comCount] = strtok(NULL, "|");
//         }

//         // Remove leading and trailing spaces from each command
//         for (int i = 0; i < comCount; i++)
//         {
//             if (commands[i][0] == ' ')
//                 commands[i]++;
//             if (commands[i][strlen(commands[i]) - 1] == ' ')
//                 commands[i][strlen(commands[i]) - 1] = '\0';
//         }

//         // Count number of pipes and create required file descriptor pairs
//         int pipeCount = comCount - 1;
//         int fds[2 * pipeCount];

//         // Initialize the pipe file descriptors
//         for (int i = 0; i < 2 * pipeCount; i += 2)
//             if (pipe(fds + i))
//             {
//                 fprintf(stderr, "Unable to create pipe\n");
//                 exitCode = -1;
//                 return;
//             }
//         int pid;

//         // For each command
//         for (int i = 0; i < comCount; i++)
//         {
//             // Make a new child process for the shell
//             pid = fork();
//             // In the child process
//             if (pid == 0)
//             {
//                 // If not the last command, link the block's write fd to stdout
//                 if (i < pipeCount)
//                     dup2(fds[2 * i + 1], STDOUT_FILENO);
//                 // If not the first command, link the previous block's read fd to stdin to read what the previous command wrote
//                 if (i > 0)
//                     dup2(fds[2 * i - 2], STDIN_FILENO);
//                 // Close the pipes in the current child (just clean up)
//                 for (int j = 0; j < 2 * pipeCount; j++)
//                     close(fds[j]);
//                 // Execute the command
//                 char *command = commands[i];
//                 executeCommand(command,path);
//                 // Exit the process on completion
//                 exit(exitCode);
//             }
//         }
//         // Close all the pipes in the parent
//         for (int i = 0; i < 2 * pipeCount; i++)
//             close(fds[i]);
//         // Reap all the dead child processes
//         for (int i = 0; i < comCount; i++)
//         {
//             if(i == comCount - 1)
//             {
//                 wait(&exitCode);
//                 exitCode = WEXITSTATUS(exitCode);
//             }
//             else
//                 wait(NULL);
//         }
//     }
// }