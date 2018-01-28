/*
 * Every delay seconds program reads from stdio and counts the number of words
 * (words are sequences of characters delimited with whitespaces).
 */
#define _POSIX_C_SOURCE 199309L
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <ctype.h>
#include <err.h>
#include <bits/siginfo.h>

volatile static unsigned int answer;
static unsigned int delay; /* No need to be volatile, because is set before any alarms and not changes after that. */

int newword(char a, char b)
{
    return (isspace(b) && !isspace(a));
}

static void handler(int sig)
{
    char pv[12];
    struct sigset_t new_mask, old_mask;
    sigprocmask(SIG_SETMASK, NULL, &old_set);
    sigemptyset(&newmask);
    sigorset(&new_mask, &new_mask, &old_mask);
    sigaddset(&newmask, SIGINT);
    sigprocmask(SIG_SETMASK, &new_mask, NULL);

    if (sig == SIGALRM)
    {
        sprintf(pv, "%d\n",answer);
        write(1, pv, strlen(pv));
    }
    signal(SIGALRM, handler);
    alarm(delay);
    sigprocmask(SIG_SETMASK, &oldmask, NULL);
}

int main(int argc, char* argv[])
{
    unsigned char delay;
    int read = ' ', last = ' ';
    if (argc != 2)
    {
        errx(3, "argc != 2");
    }
    delay = atoi(argv[1]);

    signal(SIGALRM, handler );
    alarm(delay);

    while (read != EOF)
    {
        if (newword(read, last))
        {
            ++answer;
        }
        last = read;
        read = fgetc(stdin);
    }
    printf("%d\n", ans);

    return 0;
}
