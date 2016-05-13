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
#include "error_stuff.h"

/** Process builtin functions, such as exit, cd (unimplemented). */
int builtin_func(proc* pr)
{
	if (!strcmp(pr->argv[0], "exit")) {
		exit(1);
	} else return 0;
}

/** Redirect streams of process pr */
int redirect(proc* pr)
{
	/* background stuff */
	if (pr->in == STDIN_FILENO && pr->backgr) {
		/* original shell don't redirect output to /dev/null */
		/*pr->outfile = */pr->infile = "/dev/null";
	}

	/* input stuff */
	if (pr->infile != 0) {
		NFN1(pr->in = open(pr->infile, O_RDONLY, 0), E_FILE_OPEN);
	}
	CN1(dup2(pr->in, STDIN_FILENO), E_DUP2);
	if (pr->in != STDIN_FILENO) {
		CN1(close(pr->in), E_CLOSE);
	}

	/* output stuff */
	if (pr->outfile != 0) {
		int flags = O_WRONLY | O_CREAT;
		if (pr->append) {
			flags |= O_APPEND;
		} else {
			flags |= O_TRUNC;
		}
		NFN1(pr->out = open(pr->outfile, flags, 0700), E_FILE_OPEN);
	}
	CN1(dup2(pr->out, STDOUT_FILENO), E_DUP2);
	if (pr->out != STDOUT_FILENO) {
		CN1(close(pr->out), E_CLOSE);
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
	CN1( pid = fork(), E_FORK );
	if( pid == 0 ) {
		/* child */
		if (fd_to_close != -1) {
			CN1(close(fd_to_close), E_CLOSE);
		}
		char *cmdname, *cmdpath;
		redirect(pr);
		cmdname = strrchr( pr->argv[0], '/' );
		if( cmdname == NULL) {
			cmdname = pr->argv[0];
		} else ++cmdname;
		cmdpath = pr->argv[0];
		pr->argv[0] = cmdname;
		CN1( execvp( cmdpath, pr->argv ), E_EXECVP );
		RAISEX(E_UNREACHABLE);
	}
	/* parent */
	if (pr->in>STDIN_FILENO)
		CN1(close(pr->in), E_CLOSE);
	if (pr->out>STDOUT_FILENO)
		CN1(close(pr->out), E_CLOSE);
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
			} else RAISE(E_WAITPID);
		}
		/* debug */
		/*if (WIFEXITED(status)) {
			printf( "process %d finished with code %d\n", wpid, WEXITSTATUS( status ));
		} else if (WIFSIGNALED(status)) {
			printf( "process %d killed with signal %d\n", wpid, WTERMSIG( status ));
		}*/
	} while (wpid != pid);

}