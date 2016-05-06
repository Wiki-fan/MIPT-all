#define _POSIX_C_SOURCE 200901L
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <err.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <netdb.h>
#include <unistd.h>
#include <strings.h>
#include <getopt.h>

#include "../common/utils.h"
#include "game_stuff.h"
#include "tty_stuff.h"
#include "config_stuff.h"
#include "net_stuff.h"
#include "common_types.h"
#include "server_stuff.h"

#define FILENAME_MAX_LEN 256

static void handler( int sig )
{
	LOG(("Exiting on signal SIGTERM"));
}

int main( int argc, char* argv[] )
{
	int c;
	int daemonize = 0;
	int optind = 0;
	char log[FILENAME_MAX_LEN];
	char config[FILENAME_MAX_LEN];
	struct sigaction sa;

	/* Default values. */
	strcpy( log, "log_file.txt" );
	strcpy( config, "config.cfg" );

	/* Parse command line options */
	do {
		static struct option long_options[] =
				{
						{ "help",      no_argument,       0, 'h' },
						{ "version",   no_argument,       0, 'v' },
						{ "daemonize", no_argument,       0, 'd' },
						{ "config",    required_argument, 0, 'c' },
						{ "log",       required_argument, 0, 'l' },
						{ 0, 0,                           0, 0 }
				};
		c = getopt_long( argc, argv, "hvl:c:d", long_options, &optind );

		switch( c ) {
			case 'h':
				puts( "Usage:\n./server [--daemonize] [-c config_file] [-l log_file]\n./server -h\n./server -v" );
				exit( 0 );
			case 'v':
				puts( "Version 0.1" );
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

	LOG(( "Starting server" ));
	LOG(( "Reading config from %s", config ));
	LOG(( "Writing log to %s", log ));
	LOG(( "Daemonize: %s", daemonize ? "Yes" : "No" ));

	/* Setting signal handler */
	memset( &sa, 0, sizeof( sa ));
	sa.sa_handler = handler;
	sigfillset( &sa.sa_mask );
	sa.sa_flags = SA_RESTART;

	if( sigaction( SIGTERM, &sa, NULL) == -1 )
	{
		err( 20, "Error setting signal handler" );
	}

	read_config_from_file( config );
	server_start();

	return 0;
}