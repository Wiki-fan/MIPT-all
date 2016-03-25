#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>

static void handler( int signum )
{
    /*do nothing */
}

int main()
{
    struct sigaction sa;
    int i;
    sigset_t set;
    siginfo_t info;
    sigfillset( &set ); /* All signals. */

    /* Set our handler. */
    sa.sa_handler = handler;
    /* Don't block any other signals. */
    sigemptyset( &sa.sa_mask );
    for( i = 1; i < 32; ++i )
    {
        if( sigismember( &set, i ))   /* If signal i really exists */
        {
            if( sigaction( i, &sa, NULL) == -1 )
            {
                /*err( 3, "Unable to set handler" );*/
            }
        }
    }

    for( i = 0; i < 10; ++i )
    {
        sigwaitinfo( &set, &info );
        printf( "%d\n", info.si_pid );
    }


    return 0;
}
