#include <signal.h>
#include <stdio.h>
#include <sys/socket.h>
#include "net_stuff.h"
#include "server_stuff.h"
#include "timer_stuff.h"

extern int exitcode;
extern timer_t timerid;

int port;
int main()
{
    port = 1255;

    LOG(( "Starting server" ));

    timerid = create_timer();

    server_start();

    return exitcode;
}
