#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <err.h>
#include <fcntl.h>
#include <errno.h>
#include "../common/utils.h"
#include "proc_stuff.h"
#include "parser_stuff.h"

void checkopenedfd()
{
	int i;
	for (i = 3; i<20; ++i) {
		if (fcntl(i, F_GETFL) != -1 || errno != EBADF) {
			fprintf(stderr, "Descriptor %d is open\n", i);
		}
	}
}
int main()
{
	int pid;
	TOKEN token = T_NL;
	while( 1 ) {
		if( token == T_NL ) {
			printf( PROMPT );
		}
		token = process(&pid, 0, 0 );
		if (token == T_END)
			break;
		wait_and_display(pid);
		checkopenedfd();
	}
	return 0;
}