#include "headers.h"
#include "functions.h"

void promptInit()
{
    printf("Welcome to my shell\n");
}

void prompt(char* path)
{
    struct utsname unameBuffer;
    errno = 0;
    if (uname(&unameBuffer) < 0)
    {
        perror("uname");
        exit(EXIT_FAILURE);
    }

    char *username = getpwuid(getuid())->pw_name;

    printf("\033[1;36m<%s@%s:%s\033[0m\033[1;33m%s%s>\033[0m $ ", username, unameBuffer.sysname, "", Tildify(path), extension);
    fflush(stdout);    
}