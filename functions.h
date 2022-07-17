#ifndef FUNCTIONS_HEADER
#define FUNCTIONS_HEADER 

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

void redirectIn(char *fileName);

void redirectOut(char *fileName);

int separator(char c);

void split(char *string, char*** tokens, char** separators);

int positionNull(char **command, int times);

void createPipe(char *args[]);

void run(char *args[]);

void catch_int(int sig_num);

#endif
