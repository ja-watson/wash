/* include guard */
#ifndef WASH_BOOTSTRAP_H
#define WASH_BOOTSTRAP_H

/* sfl headers */
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

/* program definitions */
#define LINE_LEN        80
#define MAX_ARGS        64
#define MAX_ARG_LEN     16
#define MAX_PATHS       64
#define MAX_PATH_LEN    96
#define WHITESPACE      " .,\t\n"
#define PATHSEP         ":"

#ifndef NULL
#define NULL 0
#endif

/* program structures (objects) */
struct command_t
{
    char*   name;
    int     argc;
    char*   argv[MAX_ARGS];
};

/* program function declarations */

/* locate path that contains executable? */
char* lookupPath(char** argv, char** dir)
{
    /* local variable declaration */
    char*   result;

    /* check for absolute path name */
    if( *argv[0] == '/' )
    {
        return argv[0];
    }

    /* search for */
    for(int i = 0; i < MAX_PATHS; i++)
    {
        if ( dir[i] != 0 )
        {
            /* init/reinit scope variables */
            int     first           = strlen(dir[i]);
            int     second          = strlen(*argv);
            int     fullPathLength  = first + second + 1;
            char    fullPath[fullPathLength];

            strcpy(fullPath, dir[i]);
            strcat(fullPath, "/");
            strcat(fullPath, argv[0]);

            if( access(fullPath, X_OK) == 0 )
            {
                result = fullPath;
                return result;
            }
        }
        else
        {
            break;
        }
    }

    /* log error and return null */
    // fprintf(stderr, "\n\n%s: command not found\n\n", argv[0]);
    return NULL;
}

/* parse command and build parameter list */
void parseCommand(char * cLine, struct command_t* cmd)
{
    /* local variable declarations */
    int     argc;
    char**  clPtr;

    /* initialization */
    clPtr           = &cLine;         /* cLine is the command line read in by readCommand() function */
    argc            = 0;

    /* fill argv[] */
    for(int i = 0; i < MAX_ARGS; i++)
    {
        /* copy item from command line to argument index */
        cmd->argv[argc] = (char*) malloc(MAX_ARG_LEN);
        cmd->argv[argc] = strsep(clPtr, WHITESPACE);

        /* test for break state */
        if ( cmd->argv[argc] == 0 )
        {
            break;
        }

        argc++;
    }

    /* set the command name and argc */
    cmd->argc = argc-1;
    cmd->name = (char*) malloc(sizeof(cmd->argv[0]));
    strcpy(cmd->name,cmd->argv[0]);
}

void parsePath(char* dirs[])
{
    /* local variable declarations */
    char*   pathEnvVar;
    int     index;
    char**  envPath;

    /* initializations */
    index       = 0;
    pathEnvVar  = (char*) getenv("PATH");
    envPath     = &pathEnvVar;

    /* loop to parse thePath and fill dirs[] | pathv */
    for(int i = 0; i < MAX_PATHS; i++ )
    {
        /* copy item from thePath to dirs index */
        dirs[i] = (char*) malloc(MAX_PATH_LEN);
        dirs[i] = strsep(envPath, PATHSEP);

        /* test for early break state */
        if ( dirs[index] == 0 )
        {
            break;
        }

        index++;
    }
}

/* print command line prompt */
void printPrompt()
{
    printf("%s:>", "WASH");
}

/* read input from the command line */
void readCommand(char * buffer)
{
    char* line = fgets(buffer, LINE_LEN, stdin);

    char* nl = strchr( buffer, '\n' ) ;

    if( nl != 0 )
    {
        /* Replace newline character with null */
        *nl = 0 ;
    }
    else
    {
        /* If buffer was shorter than the line - read and discard rest of line. */
        while( getchar() != '\n' ) { /* do nothing */ }
    }
}

#endif //WASH_BOOTSTRAP_H