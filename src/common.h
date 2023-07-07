// Header file for all c files

// Global shared libraries
#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<dirent.h>
#include<errno.h>
#include<termios.h>
#include<signal.h>

// Initialize global variables
extern char **environ;
extern int errno;
