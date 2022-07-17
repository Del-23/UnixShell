    #include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "functions.h"
#include <signal.h>

int main()
{
    //------------------------------------------------------------------------
    //  DECLARING VARIABLES
    //------------------------------------------------------------------------
    char str[101]; // input

    char *separators; // symbols that indicate redirecting or piping
    char **command;   // instructions in the input string

    char **backup_command; // backup to ensure proper deallocation

    int valuePositionNull; // auxiliary variable
    int separator;         // auxiliary variable
    int times;             // auxiliary variable

    signal(SIGINT,catch_int);
    signal(SIGSTOP, catch_int);
    
    //------------------------------------------------------------------------
    //  MINISHELL
    //------------------------------------------------------------------------
    printf("Welcome to my miniature-shell\n");

    while (1)
    {
        printf("UnixShell_Marina>");
        str[0] = 0;

        // reading input
        if(scanf("%100[^\n]%*c", str) < 1){
            getchar();
            continue; 
        }

        command = (char **)malloc(100 * sizeof(char *));
        separators = (char *)malloc(100 * sizeof(char));

        backup_command = command;
        separator = 0;
        times = 1;

        split(str, &command, &separators); // parsing input and creating token vector and separator vector

        if (separators[0] == 0) // case of a unique command, as "ls", without any redirecting symbol
        {
            run(command);
        }
        else // case with any redirecting symbol
        {
            while (separators[separator] != 0)
            {
                valuePositionNull = positionNull(command, times++);
                switch (separators[separator])
                {
                case '>':
                {
                    redirectOut(command[valuePositionNull + 1]);
                    break;
                }

                case '<':
                {
                    redirectIn(command[valuePositionNull + 1]);
                    break;
                }

                case '|':
                {
                    createPipe(command);
                    command = &command[valuePositionNull + 1]; // shift command vector to the right side of the pipe
                    times = 1;
                    break;
                }
                }
                separator++;
            }
            run(command);
        }
        free(backup_command);
        free(separators);
    }
    return 0;
}
