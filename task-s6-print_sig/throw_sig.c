#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>

int main(int argc, char* argv[])
{
    pid_t pid;
    int i;
    sscanf(argv[1], "%d", &pid);

    printf("%d", pid);
    for( i = 0; i < 10; ++i )
    {
        kill(pid, SIGUSR1);
        /* Refuses to work without next line. Why?.. */
        system("pause 500");
    }

    return 0;
}