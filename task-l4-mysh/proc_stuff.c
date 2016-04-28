#include "proc_stuff.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <err.h>
#include <fcntl.h>
#include <string.h>
#include <wait.h>
#include "../common/utils.h"

/* redirects streams of process pr */
int redirect(proc* pr)
{
	/* input stuff */
	if (pr->infile != 0) {
		CHN1(pr->in = open(pr->infile, O_RDONLY, 0), 29, "Error opening file");
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
		CHN1(pr->out = open(pr->outfile, flags, 0700), 29, "Error opening file");
	}
	CHN1(dup2(pr->out, STDOUT_FILENO), 30, "Dup2 failed");
	if (pr->out != STDOUT_FILENO) {
		CHN1(close(pr->out), 31, "Can't close file");
	}
}

/* runs process and redirects input, returns new fd */
int run( proc* pr, int fd_to_close)
{
	pid_t pid;
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
	return pid; /* returns child pid */
}

int wait_and_display(int pid)
{
	int status = 0;
	do {
		CHN1(waitpid(pid, &status, 0), 33, "waitpid error");
	} while (!WIFEXITED(status) && !WIFSIGNALED(status));

}