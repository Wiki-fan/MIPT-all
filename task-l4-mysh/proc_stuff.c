#include "proc_stuff.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <err.h>
#include <fcntl.h>
#include <string.h>
#include <wait.h>
#include <errno.h>
#include "../common/utils.h"

#define DROP( COND, MSG ) \
    if (COND) fprintf(stderr, "%s: %s\n", MSG, strerror(errno));\
	return 0;

int builtin_func(proc* pr)
{
	if (!strcmp(pr->argv[0], "exit")) {
		exit(1);
	} else return 0;
}

/* redirects streams of process pr */
int redirect(proc* pr)
{
	/* background stuff */
	if (pr->in == STDIN_FILENO && pr->backgr) {
		/* original shell don't redirect output to /dev/null */
		/*pr->outfile = */pr->infile = "/dev/null";
	}

	/* input stuff */
	if (pr->infile != 0) {
		DROP(pr->in = open(pr->infile, O_RDONLY, 0), "Error opening file");
	}
	CHN1(dup2(pr->in, STDIN_FILENO), 30, "Dup2 failed");
	if (pr->in != STDIN_FILENO) {
		CHN1(close(pr->in), 31, "Can't close file");
	}

	/* output stuff */
	if (pr->outfile != 0) {
		int flags = O_WRONLY | O_CREAT;
		if (pr->append) {
			flags |= O_APPEND;
		} else {
			flags |= O_TRUNC;
		}
		DROP(pr->out = open(pr->outfile, flags, 0700), "Error opening file");
	}
	CHN1(dup2(pr->out, STDOUT_FILENO), 30, "Dup2 failed");
	if (pr->out != STDOUT_FILENO) {
		CHN1(close(pr->out), 31, "Can't close file");
	}
}

/* runs process and redirects input, returns new fd */
pid_t run( proc* pr, int fd_to_close)
{
	pid_t pid;
	/* if command is empty or builtin */
	if (pr->argc == 0 || builtin_func(pr)) {
		return 0;
	}
	CHN1( pid = fork(), 24, "Can't fork process" );
	if( pid == 0 ) {
		/* child */
		if (fd_to_close != -1) {
			CHN1(close(fd_to_close), 33, "Error closing fd");
		}
		char *cmdname, *cmdpath;
		redirect(pr);
		cmdname = strrchr( pr->argv[0], '/' );
		if( cmdname == NULL) {
			cmdname = pr->argv[0];
		} else ++cmdname;
		cmdpath = pr->argv[0];
		pr->argv[0] = cmdname;
		CHN1( execvp( cmdpath, pr->argv ), 25, "Execvp failed" );
		err( 3, "Unreachable code reached" );
	}
	/* parent */
	if (pr->in>STDIN_FILENO)
		close(pr->in);
	if (pr->out>STDOUT_FILENO)
		close(pr->out);
	/* print pid for process that runs in background */
	if (pr->backgr) {
		printf("[] %d\n", pid);
	}
	return pid; /* returns child pid */
}

void wait_and_display(int pid)
{
	pid_t wpid;
	int status = 0;
	do {
		if ((wpid = waitpid(-1, &status, 0)) == -1) {
			if (errno == ECHILD) {
				return;
			} else err(33, "waitpid error");
		}
		/* debug */
		/*if (WIFEXITED(status)) {
			printf( "process %d finished with code %d\n", wpid, WEXITSTATUS( status ));
		} else if (WIFSIGNALED(status)) {
			printf( "process %d killed with signal %d\n", wpid, WTERMSIG( status ));
		}*/
	} while (wpid != pid);

}