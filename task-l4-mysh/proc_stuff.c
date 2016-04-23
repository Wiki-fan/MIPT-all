#include "proc_stuff.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <err.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include "../common/utils.h"

int run( char* argv[], proc* fd )
{
	pid_t pid;
	CHN1( pid = fork(), 24, "Can't fork process" );
	if( pid == 0 ) {
		/* child */
		char* cmdname, * cmdpath;
		printf( "Child\n" );
		fflush( stdout );
		cmdname = strrchr( argv[0], '/' );
		if( cmdname == NULL) {
			cmdname = argv[0];
		} else ++cmdname;
		cmdpath = argv[0];
		argv[0] = cmdname;
		printf("|%s %s|\n", argv[0], argv[1]);
		CHN1( execvp( cmdpath, argv ), 25, "Execvp failed" );
		err( 3, "Unreachable code reached" );
	} else {
		/* parent */
		int status = 0;
		printf( "Parent\n" );
		waitpid( pid, &status, NULL);
		if( WIFEXITED( status )) {
			printf( "Process successfully terminated with code %d.\n", WEXITSTATUS( status ));
		} else if( WIFSIGNALED( status )) {
			printf( "Crash of child process with signal %d\n", WTERMSIG( status ));
		}
	}
	return 0;
}

int redirect(proc* pr)
{

}