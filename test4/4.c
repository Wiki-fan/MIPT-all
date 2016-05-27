#include <err.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

int main( int argc, char** argv )
{
    char* str[256];
    pid_t pr;
    pid_t pr2;
    pid_t pr3;
    int status2, status3;

    if(( pr = fork()) == -1 )
    {
        err( 2, "Can't fork process" );
    }

    if( pr == 0 )
    {
        /* pr1 */
        int infd = open( argv[3], O_RDONLY );
        printf( "%s\n", argv[3] );
        if( infd == -1 )
        {
            err( 5, "open" );
        }
        dup2( infd, STDIN_FILENO );
        close( infd );
        execlp( argv[1], argv[1], argv[2], NULL);
        err( 3, "Something went wrong. We shouldn't get to this code" );
    }
    /* parent general */
    int status;
    int pipe1[2];
    waitpid( pr, &status, 0 );
    if( WIFEXITED( status ) && (WEXITSTATUS( status ) == 0 ))
    {
        printf( "Everything as planned. Process terminated with code 0.\n" );
    }
    else
    {
        printf( "pr1 exited badly" );
        return 0;
    }
    if(( pr2 = fork()) == -1 )
    {
        err( 2, "Can't fork process" );
    }
    if( pr2 == 0 )
    {
        /* pr2 */
        if( pipe( pipe1 ))
        {
            err( 1, "Error opening pipe" );
        }
        if( dup2( pipe1[1], STDOUT_FILENO ) == -1 )
        {
            err( 6, "dup2" );
        }

        if(( pr3 = fork()) == -1 )
        {
            err( 2, "Can't fork process" );
        }
        if( pr3 == 0 )
        {
            /* pr3 */
            int offd;
            fprintf( stderr, "Got here\n" );
            if( (offd = open( argv[6], O_APPEND, 0700 )) == -1 )
            {
                err( 7, "open out" );
            }
            if( dup2( offd, STDOUT_FILENO ) == -1 )
            {
                err( 6, "dup2" );
            }
            close( offd );
            fprintf( stderr, "Got here\n" );
            if( dup2( pipe1[0], STDIN_FILENO ) == -1 )
            {
                err( 6, "dup2" );
            }
            close( pipe1[0] );
            close( pipe1[1] );
            execlp( argv[5], argv[5], NULL);
            err( 3, "Something went wrong. We shouldn't get to this code" );
        }
        /* pr2 forking */
        close( pipe1[0] );
        close( pipe1[1] );

        execlp( argv[4], argv[4], NULL);
        err( 3, "Something went wrong. We shouldn't get to this code" );

    }
    else
    {
        waitpid( pr2, &status2, 0 );
        waitpid( pr3, &status3, 0 );
    }


    return 0;
}
