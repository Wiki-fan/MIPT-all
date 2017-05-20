#define _POSIX_C_SOURCE 199309L
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <err.h>
#include <sys/socket.h>
#include <getopt.h>
#include <time.h>
#include "../common/utils.h"
#include "net_stuff.h"
#include "common_types.h"
#include "server_stuff.h"
#include "timer_stuff.h"

extern int exitcode;
extern timer_t timerid;

int port;
int main()
{
    port = 1254;

    LOG(( "Starting server" ));

    timerid = create_timer();

    server_start();

    return exitcode;
}
