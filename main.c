/*
 *
 * Name:        Joseph Watson
 * Class:       Operating Systems
 * Section:     4334 01
 * Semester:    Spring 2018
 * Program:     Programming Project One
 * Credits:     As per the lab exercise most of the code was put together from the lab 2.1 and section 2.4 examples
 *              with custom improvements made by myself, as well as fixing the book's buggy code.
 */

/* project headers */
#include "bootstrap.h"

/* program entry point*/
int main(int argc, char* argv[])
{
    /* shell initialization */
    int                 pid, numChildren;
    //int                 status;
    char*               pathv[MAX_PATHS];
    char                commandLine[LINE_LEN];
    struct command_t    command; // (struct command_t *) MALLOC(sizeof(struct command_t));

    /* get directory paths from PATH */
    parsePath(pathv);

    /* print welcome message*/
    printf("\n\n%s\n\n", "Welcome to the WAtson SHell, use at your own risk!");

    /* command loop : break on command "exit" */
    while( 1 ) //   | causes break if command is null
    {
        /* display shell prompt */
        printPrompt();

        /* read the command line and parse it */
        readCommand(commandLine);

        /* parse the command line entered*/
        parseCommand(commandLine, &command);

        /* if command.name == exit -> early termination */
        if (strcmp(command.name,"exit") == 0)
        {
            break;
        }

        /* get the full path name for the file */
        command.name = lookupPath(command.argv, pathv);

        /* check for command error and notify user */
        if ( command.name == NULL )
        {
            /* display error */
            printf("\n\n%s\n\n", "Error: Bad or Missing Command!");
            continue;
        }

        /* create child and execute the command */
        if( pid = fork() == 0 )
        {
            execv(command.name,command.argv);
            exit(0);
        }

        /* wait for the child to terminate */
        wait(NULL);
    }

    /* shell termination */
    printf("\n\n%s\n","Exiting shell...");
    return EXIT_SUCCESS;
}