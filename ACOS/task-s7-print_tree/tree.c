#define _POSIX_C_SOURCE 200901L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <err.h>
#include "../common/utils.h"

/*      0
 *    / | \
 *    1 2 3
 *     / \ \
 *     4 5 6
*/

#define ASK_WRITE_TO_PIPE(PID) kill(PID, SIGUSR2)
#define ASK_PRINT(PID) kill(PID, SIGUSR1)
#define SAY_FINISHED() kill(p[0], SIGUSR1)

pid_t p[7];
pid_t np[7];
int pipe_fd[2];
static volatile int received_response = 0;
static volatile int write_to_pipe = 0;
static volatile int need_to_print = 0;

/* Waits for SIGUSR1 and sets flag */
void parent_handler( int sig )
{
    if (sig == SIGUSR1)
    {
        received_response = 1;
        signal( SIGUSR1, parent_handler );
    }
}

void write_to_pipe_handler( int sig )
{
    if (sig == SIGUSR2)
    {
        write_to_pipe = 1;
        signal( SIGUSR2, write_to_pipe_handler );
    }
}

void need_to_print_handler( int sig )
{
    if (sig == SIGUSR1)
    {
        need_to_print = 1;
    }
}

void print_pids( int my )
{
    int j;
    for (j = 0; j < 7; ++j)
    {
        if (j == my)
        {
            printf("*");
        }
        else
        {
            printf(" ");
        }
        printf("p%d=%ld ", j, (long)p[j]);
    }
    printf("\n");
}

void print_and_die( int i )
{
    read(pipe_fd[0], p, sizeof(pid_t) * 7);
    print_pids( i );
    close(pipe_fd[1]);
    close(pipe_fd[0]);
    SAY_FINISHED();
    exit(0);
}

int main()
{
    int i;
    p[0] = getpid();
    CHN1(pipe(pipe_fd), 1, "Pipe");
    signal( SIGUSR1, need_to_print_handler );
    signal( SIGUSR2, write_to_pipe_handler );

    /* Wait for 1 to print */
    p[1] = fork();
    if (p[1] == 0)
    {
        while (1)
        {
            if (need_to_print)
                print_and_die( 1 );
            pause();
        }
    }

    p[2] = fork();
    if (p[2] == 0)
    {
        /* Wait for 4 to print */
        p[4] = fork();
        if (p[4] == 0)
        {
            while (1)
            {
                if (need_to_print)
                    print_and_die( 4 );
                pause();
            }
        }

        /* Wait for 5 to print */
        p[5] = fork();
        if (p[5] == 0)
        {
            while (1)
            {
                if (need_to_print)
                    print_and_die( 5 );
                pause();
            }
        }

        /* Wait for 2 to print */
        while (1)
        {
            if (need_to_print)
                print_and_die( 2 );
            /* Transmit message */
            if (write_to_pipe)
            {
                write(pipe_fd[1], p, sizeof(pid_t) * 7);
                SAY_FINISHED();
                write_to_pipe = 0;
            }
            pause();
        }
    }

    p[3] = fork();
    if (p[3] == 0)
    {
        /* Wait for 6 to print */
        p[6] = fork();
        if (p[6] == 0)
        {
            while (1)
            {
                if (need_to_print)
                    print_and_die( 6 );
                pause();
            }
        }

        /* Wait for 3 to print */
        while (1)
        {
            if (need_to_print)
                print_and_die( 3 );
            /* Transmit message */
            if (write_to_pipe)
            {
                write(pipe_fd[1], p, sizeof(pid_t) * 7);
                SAY_FINISHED();
                write_to_pipe = 0;
            }
            pause();
        }
    }

    /* Root process routine */
    signal( SIGUSR1, parent_handler );

    ASK_WRITE_TO_PIPE(p[2]);
    while (!received_response)
    {
        pause();
    }
    read(pipe_fd[0], np, sizeof(pid_t) * 7);
    p[4] = np[4];
    p[5] = np[5];
    received_response = 0;

    ASK_WRITE_TO_PIPE(p[3]);
    while (!received_response)
    {
        pause();
    }
    read(pipe_fd[0], np, sizeof(pid_t) * 7);
    p[6] = np[6];

    print_pids( 0 );
    for (i = 1; i < 7; ++i)
    {
        write(pipe_fd[1], p, sizeof(pid_t) * 7);
        received_response = 0;
        ASK_PRINT(p[i]);
        while (!received_response)
        {
            pause();
        }
    }

    close(pipe_fd[0]);
    close(pipe_fd[1]);
    return 0;
}
