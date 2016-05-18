#define _POSIX_C_SOURCE 200901L
#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <signal.h>
#include <memory.h>
#include "proc_stuff.h"
#include "parser_stuff.h"
#include "error_stuff.h"

static void* handler(int signum)
{
	switch(signum) {
		case SIGINT:
			break;
		case SIGTERM:
			break;
		default:
			RAISEX(E_UNSUPPORTED_SIGNAL);
	}
}

int main()
{
	int pid;
	TOKEN token = T_NL;
	struct sigaction sa;
	/* Setting signal handler */
	memset( &sa, 0, sizeof( sa ));
	sa.sa_handler = handler;
	sigfillset( &sa.sa_mask );
	sa.sa_flags = SA_RESTART;
	CN1( sigaction( SIGTERM, &sa, NULL ), E_SIGACTION );
	CN1( sigaction( SIGINT, &sa, NULL ), E_SIGACTION );

	while( 1 ) {
		if( token == T_NL || token == T_ERROR ) {
			printf( PROMPT );
		}
		token = process(&pid, 0, 0 );
		if (token == T_END)
			break;
		if (token != T_ERROR)
			wait_and_display(pid);
	}
	return 0;
}