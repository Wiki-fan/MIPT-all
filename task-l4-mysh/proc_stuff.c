#include "proc_stuff.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <err.h>
#include <fcntl.h>
#include "../common/utils.h"

int run(char* argv[], stdfd* fd)
{
	pid_t pid;
	CHN1(pid=fork(), 24, "Can't fork process");
	if (pid == 0) {
		/* child */
		CHN1(execvp(argv[0], argv), 25, "Execvp failed");
		err(3, "Unreachable code reached");
	} else {

	}
}