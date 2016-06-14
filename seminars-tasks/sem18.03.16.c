#include <unistd.h>
#include <err.h>
#include <fcntl.h>
#include <sys/wait.h>
int main()
{
    int pipe1[2], pipe2[2];
    pid_t pr1, pr2, pr3;
    int status1, status2, status3;
    if( pipe( pipe1 ))
    {
        err( 1, "Error opening pipe1" );
    }
    if( pipe( pipe2 ))
    {
        err( 2, "Error opening pipe2" );
    }
    pr1 = fork();
    if( pr1 == -1 )
    {
        err( 3, "Can't fork process" );
    }
    if( pr1 == 0 )
    {
        dup2( pipe1[1], 1 );
        close(pipe1[0]);
        close(pipe1[1]);
        execlp( "ls", "ls", NULL);
        err( 4, "We should not get here, execlp not working" );
    }
    pr2 = fork();
    if( pr2 == -1 )
    {
        err( 3, "Can't fork process" );
    }
    if( pr2 == 0 )
    {
        dup2( pipe1[0], 0 );
        dup2( pipe2[1], 1 );
        close(pipe1[0]);
        close(pipe1[1]);
        close(pipe2[0]);
        close(pipe2[1]);

        execlp( "tee", "tee", "foo", NULL);
        err( 4, "We should not get here, execlp not working" );
    }
    pr3 = fork();
    if( pr3 == -1 )
    {
        err( 3, "Can't fork process" );
    }
    if( pr3 == 0 )
    {
        dup2( pipe2[0], 0 );
        execlp( "sort", "sort", NULL);
        err( 4, "We should not get here, execlp not working" );
    }
    close(pipe1[0]);
    close(pipe2[0]);
    close(pipe1[1]);
    close(pipe2[1]);
    waitpid( pr1, &status1, 0 );
    waitpid( pr2, &status2, 0 );
    waitpid( pr3, &status3, 0 );
    if( status1 || status2 || status3 )
    {
        return 5;
    }
    return 0;

}