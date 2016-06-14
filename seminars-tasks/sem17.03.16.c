#include <err.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

int main( int argc, char** argv)
{
    char* str[256];
    pid_t pr;
    if (argc != 2)
    {
        errx(1, "Wrong number of parameters");
    }

    if ((pr = fork()) == -1)
    {
        err(2, "Can't fork process");
    }
    /* Son process. */
    if (pr == 0)
    {
        strcpy( str, "ls -la /home/" );
        strcat( str, argv[1] );
        execlp( "ls", "ls", "-la", str, NULL);
        err(3, "Something went wrong. We shouldn't get to this code");
    }
    else
    {
        int status;
        if (WIFEXITED(status) && (WEXITSTATUS(status) == 0))
        {
            printf("Everything as planned. Process terminated with code 0.\n");
            return 0;
        }

    }
}