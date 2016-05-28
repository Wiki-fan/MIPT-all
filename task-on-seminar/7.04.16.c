/*
 *  отец
 * |  |  |
 * p1 p2 p3
 * Распечатать из любого из данных процессов эту иерархию в правильном порядке. Звёздочкой отмечать текущий процесс.
 */
#define _POSIX_C_SOURCE 200901L
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <bits/signum.h>
#include <signal.h>
#include <wait.h>

volatile pid_t p0, t, p1,p2,p3, id;
volatile int pipefd[2];
volatile char succ = 0;

void hndlr1(int signal)
{
    if (signal == SIGUSR1)
    {
        read(pipefd[0], &p2, sizeof(pid_t));
        read(pipefd[0], &p3, sizeof(pid_t));
        print_info(p1);
        kill(p0, SIGUSR2);
        close(pipefd[0]);
        close(pipefd[1]);
        exit(0);
    }
}
void hndlr2(int signal)
{
    if (signal == SIGUSR1)
    {
        read(pipefd[0], &p3, sizeof(pid_t));
        print_info(p2);
        close(pipefd[0]);
        close(pipefd[1]);
        exit(0);
    }
}
void hndlr3(int signal)
{
    if (signal == SIGUSR1)
    {
        read(pipefd[0], &p3, sizeof(pid_t));
        print_info(p2);
        close(pipefd[0]);
        close(pipefd[1]);
        exit(0);
    }
}
void succ_hndlr(int sig)
{
    if (sig == SIGUSR2)
    {
        succ = 1;
    }
}

void print_info(pid_t pid)
{
    if (pid == p0)
    {
        fprintf(stdout, "*p0:%lu\n", (unsigned long int)pid);
    }
    else
    {
        printf("p0:%lu\n", (unsigned long int)pid);
    }
    if (pid == p1)
    {
        fprintf(stdout, "*p1:%lu\n", (unsigned long int)pid);
    }
    else
    {
        printf("p1:%lu\n", (unsigned long int)pid);
    }
    if (pid == p2)
    {
        fprintf(stdout, "*p2:%lu\n", (unsigned long int)pid);
    }
    else
    {
        printf("p2:%lu\n", (unsigned long int)pid);
    }
    if (pid == p3)
    {
        fprintf(stdout, "*p3:%lu\n", (unsigned long int)pid);
    }
    else
    {
        printf("p3:%lu\n", (unsigned long int)pid);
    }
}
int main()
{
    p0 = getpid();
    pipe(pipefd);
    p1 = fork();
    if (p1 == 0)
    {
        p1 = getpid();
        signal(SIGUSR1, &hndlr1);
        while (1)
        {
            pause();
        }
        return 0;
    }
    p2 = fork();
    if (p2 == 0)
    {
        p2 = getpid();
        signal(SIGUSR1, hndlr2);
        while (1)
        {
            pause();
        }
        return 0;
    }
    if (p3 == 0)
    {
        print_info(p3);
        return 0;
    }
    write(pipefd[1], &p2, sizeof(pid_t));
    write(pipefd[1], &p3, sizeof(pid_t));
    signal(SIGUSR2, succ_hndlr);
    kill(p1, SIGUSR1);
    while (!succ)
    {
        pause();
    }

    write(pipefd[1], &p3, sizeof(pid_t));
    kill(p2, SIGUSR1);
    wait(NULL);
    wait(NULL);
    wait(NULL);

    print_info(p0);
    return 0;
}