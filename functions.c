#include "functions.h"

/**
 * Redirects stdin from a file.
 * 
 * @param fileName the file to redirect from
 */
void redirectIn(char *fileName)
{
    int in = open(fileName, O_RDONLY);
    dup2(in, STDIN_FILENO);
    close(in);
}

/**
 * Redirects stdout to a file.
 * 
 * @param fileName the file to redirect to
 */
void redirectOut(char *fileName)
{
    int out = open(fileName, O_WRONLY | O_TRUNC | O_CREAT, 0600);
    dup2(out, STDOUT_FILENO);
    close(out);
}

/**
 * Identifies if a character is one of the listed symbols.
 * 
 * @param c char character to be verified as a match or not to the listed symbols
 * @returns boolean indicator of finding at least one listed symbol
 */
int separator (char c)
{
    switch(c)
    {
        case '>':
        case '<':
        case '|':
            return 1;
        default:
            return 0;
    }
}

/**
 * Parses the input and selects the commands and the symbols.
 * 
 * @param string string to be parsed
 * @param tokens vector of little pieces of the parsed string, result of the parsing
 * @param separators vector of symbols used to divide the string into the tokens
 */
void split(char *string, char*** tokens, char** separators)

{
    int numTokens = 0;
    int numSeparators = 0;
    char *token = strtok(string, " ");
    
    while(token != NULL)
    {
        if (separator(token[0])) //if token is a separator
        {
            (*separators)[numSeparators++] = token[0]; //add to the separators vector
            (*tokens)[numTokens++] = NULL; //add an end-of-arguments marker so execv knows where the arguments end
            token = strtok(NULL, " "); //after a separator, there must be a non-separator
            (*tokens)[numTokens++] = token; //get the next non-separator
        }
        else
        {
            (*tokens)[numTokens++] = token; //token is a non-separator, so add to tokens vector
        }
            
        token = strtok(NULL, " "); //get next token
    }    
    (*tokens)[numTokens++] = NULL; //end-of-arguments marker for the last command
    (*separators)[numSeparators++] = 0; //end-of-separators marker to use as delimiter
}

/**
 * Identifies the position of the times-th NULL.
 * 
 * @param command the input
 * @param times variable that counts how many times a NULL appears after the input has been parsed
 * @returns position of the times-th NULL in the vector
 */
int positionNull(char **command, int times)
{
    int i = 0;
    int wasNull = 0;

    while (wasNull != times) //while the number of times NULL appeared is less than expected
    {
        if(command[i] == NULL)
            wasNull++;
        i++;
    }
    return i-1; //return position of the times-th NULL entry
}

/**
 * Creates a pipe.
 * 
 * @param args [description]
 */
void createPipe(char *args[])
{
    int fd[2];
    pipe(fd);

    dup2(fd[1], 1); //changes stdout to the piped output descriptor (fd[1])
    close(fd[1]);

    run(args); //run command of the left side of the pipe

    dup2(fd[0], 0); //changes stdin to the piped input descriptor (fd[0])
    close(fd[0]);
}

/**
 * Runs a command.
 * 
 * @param *args[] the args to run
 */
void run(char *args[])
{
    pid_t pid;

    pid = fork();
    if (pid < 0) { 
        fprintf(stderr, "Failed to initialize forked process"); 
    }
    else if ( pid == 0) { /* child process */
        execv(args[0], args);
    }
    else { /* parent process */
        waitpid(pid, NULL, 0);
    }
    redirectIn("/dev/tty"); //returning stdin to the original file descriptor
    redirectOut("/dev/tty"); //returning stdout to the original file descriptor
}