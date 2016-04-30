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

int main( int argc, char* argv[] )
{
	int optind = 0;
	char log[256];
	strcpy(log, "log_file.txt");
	char config[256];
	strcpy(config, "config.cfg");

	int c;
	int daemonize = 0;

	do
	{
		/*int curind = optind;*/
		static struct option long_options[] =
				{
						{ "help",    no_argument,       0, 'h' },
						{ "version", no_argument,       0, 'v' },
						{ "daemonize", no_argument , 0, 'd' },
						{ "config", required_argument, 0, 'c'},
						{ "log", required_argument, 0, 'l'},
						{ 0, 0,                         0, 0 }
				};
		c = getopt_long(argc, argv, "hvl:c:d",
		                long_options, &optind);

		switch (c)
		{
			case 'h':
				puts ("Usage:\n./server [--daemonize] [-c config_file] [-l log_file]\n./server -h\n./server -v");
				exit(0);
			case 'v':
				puts ("Version 0.1");
				exit(0);
			case 'l':
				strncpy(log,  optarg, 256);
				break;
			case 'c':
				strncpy(config,  optarg, 256);
				break;
			case 'd':
				daemonize = 1;
				break;
			case '?':
				/* getopt_longs writes error by itself */
				/*if (optopt)  printf("Bad short opt '%c'\n", optopt);
				else  printf("Bad long opt \"%s\"\n", argv[curind]);*/
				errx(11, "Unknown parameter");

			case -1:
				break;

			default:
				err(3, "Unreachable code");
		}
	}
	while (c != -1);

	LOG(("Starting server\n"));
	LOG(("Reading config from %s\n", config));
	LOG(("Writing log to %s\n", log));
	LOG(("Daemonize: %s\n", daemonize? "Yes":"No"));

	read_config_from_file( config );
	server_init();

	return 0;
}