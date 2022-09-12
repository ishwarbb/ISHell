#include"headers.h"
#include "functions.h"

void init()
{
    strcpy(PreviousDirectory,"uwu");
    strcpy(extension,"");
    QUIT = 0;
    bgpno = 0;
    initSTDIN = 0;
    initSTDOUT = 1;
    fd1 = -1;
    fd2 = -1;
    redirread = 0;
    redirwrite = 0;
}
