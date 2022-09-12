#include "headers.h"
#include "functions.h"


void ResetStreams()
{
    dup2(initSTDIN,STDIN_FILENO);
    dup2(initSTDOUT,STDOUT_FILENO);

    if(redirread)
    {
        redirread = 0;
        dup2(initSTDIN,STDIN_FILENO);
        close(fd1);
    }
    if(redirwrite)
    {
        redirwrite = 0;
        dup2(initSTDOUT,STDOUT_FILENO);
        close(fd2);
    }
}

char* HandleRedirection(char* command)
{
    char* NewCommand = (char *) malloc(MY_LEN* sizeof(char));
    strcpy(NewCommand, " ");
   char* token = strtok(command," \t");
   if(debug) printf("Redir token = %s\n",token);
    while (token != NULL)
    {
        if(!strcmp(token,"<"))
        {
            redirread = 1;
            if(debug) printf("< detected\n");
            char* inputfile = strtok(NULL," \t");
            if(inputfile == NULL)
            {
                fprintf(stderr,"Inputfile for redirection not mentioned\n");
                return NULL;
            }
            else
            {
                // do something
                initSTDIN = dup(STDIN_FILENO);
                fd1 = open(inputfile,O_RDONLY);
                if(fd1 == -1)
                {
                    fprintf(stderr,"File %s doesnt exist\n",inputfile);
                    return NULL;
                }
                if(dup2(fd1,STDIN_FILENO) == -1)
                {
                    fprintf(stderr,"Could not redirect input to %s\n",inputfile);
                    return NULL;
                }
            }
        }
        else if(!strcmp(token,">"))
        {
            redirwrite = 1;
            if(debug) printf("> detected\n");
            char* writefile = strtok(NULL," \t");
            if(writefile == NULL)
            {
                fprintf(stderr,"Writefile for redirection not mentioned\n");
                return NULL;
            }
            else
            {
                // do something
                initSTDOUT = dup(STDOUT_FILENO);
                fd2 = open(writefile,O_WRONLY | O_TRUNC |  O_CREAT , 0644);
                if(fd2 == -1)
                {
                    fprintf(stderr,"File %s doesnt exist\n",writefile);
                    return NULL;
                }
                if(dup2(fd2,STDOUT_FILENO) == -1)
                {
                    fprintf(stderr,"Could not redirect output to %s\n",writefile);
                    return NULL;
                }
                else
                {
                    if(debug) printf("New output file = %s\n",writefile);
                }
            }
        }
        else if(!strcmp(token,">>"))
        {
            redirwrite = 1;
            if(debug) printf(">> detected\n");
            char* appendfile = strtok(NULL," \t");
            if(appendfile == NULL)
            {
                fprintf(stderr,"Appendfile for redirection not mentioned\n");
                return NULL;
            }
            else
            {
                // do something
                initSTDOUT = dup(STDOUT_FILENO);
                fd2 = open(appendfile,O_WRONLY | O_APPEND|  O_CREAT, 0644);
                if(fd2 == -1)
                {
                    fprintf(stderr,"File %s doesnt exist\n",appendfile);
                    return NULL;
                }
                if(dup2(fd2,STDOUT_FILENO) == -1)
                {
                    fprintf(stderr,"Could not redirect output to %s\n",appendfile);
                    return NULL;
                }
            }
        }
        else
        {
            if (debug) printf("Catting %s \n",token);
            strcat(NewCommand, " ");
            strcat(NewCommand,token);
            strcat(NewCommand, " ");
        }

        token = strtok(NULL," \t");
    }
    
    if(debug) printf("Redir handler returning %s \n",NewCommand); 
    return NewCommand;

}