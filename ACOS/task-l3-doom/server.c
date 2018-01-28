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
#include "game_stuff.h"
#include "tty_stuff.h"
#include "config_stuff.h"
#include "net_stuff.h"
#include "common_types.h"
#include "server_stuff.h"

#define FILENAME_MAX_LEN 256

extern Game game;
extern int exitcode;
extern timer_t timerid;

int port;
int main( int argc, char* argv[] )
{
    int c;
    int daemonize = 0;
    int optind = 0;
    char log[FILENAME_MAX_LEN] = "\0";
    char config[FILENAME_MAX_LEN] = "\0";

    port = -1;

    /* Setting random seed. */
    srand((unsigned int)time(NULL));

    /* Default values. */
    /*strcpy( log, "log_file.txt" );*/
    strcpy( config, "config.cfg" );

    /* Parse command line options */
    do
    {
        static struct option long_options[] =
        {
            { "help",      no_argument,       0, 'h' },
            { "version",   no_argument,       0, 'v' },
            { "daemonize", no_argument,       0, 'd' },
            { "config",    required_argument, 0, 'c' },
            { "log",       required_argument, 0, 'l' },
            { "port",      required_argument, 0, 'p' },
            { 0, 0,                           0, 0 }
        };
        c = getopt_long( argc, argv, "hvl:c:dp:", long_options, &optind );

        switch( c )
        {
        case 'h':
            puts( "Usage:\n./server [--daemonize] [-c config_file] [-l log_file]\n./server -h\n./server -v" );
            exit( 0 );
        case 'v':
            puts( "Version 0.2" );
            exit( 0 );
        case 'l':
            strncpy( log, optarg, FILENAME_MAX_LEN );
            break;
        case 'c':
            strncpy( config, optarg, FILENAME_MAX_LEN );
            break;
        case 'd':
            daemonize = 1;
            break;
        case 'p':
            port = atoi(optarg);
            break;
        case '?':
            /* getopt_long writes error message by itself */
            errx( 11, "Unknown parameter" );
        case -1:
            break;
        default:
            err( 3, "Unreachable code" );
        }
    }
    while( c != -1 );

    if (port == -1)
    {
        errx(E_WRONG_PARAMETERS, "You must specify the number of port");
    }

    /* Reopen stdin as soon as we specified log file. */
    if (*log != '\0')
    {
        CNULL(freopen(log, "a", stdin), E_FREOPEN);
    }

    LOG(( "Starting server" ));
    LOG(( "Reading config from %s", config ));
    LOG(( "Writing log to %s", *log == '\0'? "stdout":log ));
    LOG(( "Daemonize: %s", daemonize ? "Yes" : "No" ));

    timerid = create_timer();

    read_config_from_file( config );

    server_start();

    return 0;
}