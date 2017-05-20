#define _POSIX_C_SOURCE 200901L
#include <stdio.h>
#include <err.h>
#include <time.h>
#include <signal.h>
#include "timer_stuff.h"

/** Creates timer. Bind handler to SIGALRM on your own. */
timer_t create_timer()
{
    timer_t timerid;
    struct sigevent sev;
    /* Create the timer */
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIG;
    sev.sigev_value.sival_ptr = &timerid;
    CN1(timer_create(CLOCKID, &sev, &timerid), E_TIMER_CREATE);

    /*printf("timer ID is 0x%lx\n", (long) timerid);*/
    return timerid;
}

/** Blocks SIG signal. This function doesn't work in some reasons. */
void block_timer_signal()
{
    sigset_t mask;

    sigemptyset(&mask);
    sigaddset(&mask, SIG);
    CNON0(pthread_sigmask(SIG_BLOCK, &mask, NULL), E_SIGPROCMASK );
}

void start_timer(timer_t timerid, struct itimerspec* its )
{
    sigset_t mask;
    /* Block timer signal temporarily */
    /*printf("Blocking signal %d\n", SIG);*/
    sigemptyset(&mask);
    sigaddset(&mask, SIG);
    CNON0(pthread_sigmask(SIG_BLOCK, &mask, NULL), E_SIGPROCMASK );

    /* Start timer */
    CN1( timer_settime( timerid, 0, its, NULL ), E_TIMER_SETTIME );

    /*printf( "Unblocking signal %d\n", SIG );*/
    CNON0( pthread_sigmask( SIG_UNBLOCK, &mask, NULL ), E_SIGPROCMASK );
}

void stop_timer(timer_t timerid)
{
    /* Stop timer */
    struct itimerspec its = {{0,0}, {0,0}};
    its.it_value.tv_nsec = 0;
    its.it_value.tv_sec =0;
    CN1( timer_settime( timerid, 0, &its, NULL ), E_TIMER_SETTIME );
}
