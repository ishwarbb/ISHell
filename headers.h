#ifndef ISH
#define ISH

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/utsname.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>
#include <pwd.h>
#include <time.h>
#include <assert.h>
#include <signal.h>
#include <termios.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

#define MY_LEN 10000
#define debug 0
#define pdebug 0
extern char HomeDirectory[MY_LEN];
extern char PreviousDirectory[MY_LEN];
extern char hispath[MY_LEN];
extern char extension[MY_LEN];
extern int QUIT;
extern int errno;
extern char *bgp[MY_LEN];
extern int bgpid[MY_LEN];
extern int exists[MY_LEN];
extern int bgpno;
extern int initSTDIN;
extern int initSTDOUT;
extern int fd1;
extern int fd2;
extern int redirread;
extern int redirwrite;

#endif 


