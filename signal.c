#include "headers.h"
#include "functions.h"

void controlZ(int signum) {
    printf("\n");
    fflush(stdout);
    
    return;
}

void controlC(int signum) {
    printf("\n");
    exit(0);
}

void bindSignals()
{
    signal(SIGINT,controlC);
    signal(SIGTSTP,controlZ);
}

void sig(char* command)
{
    
}
