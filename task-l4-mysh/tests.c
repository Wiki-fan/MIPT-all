#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <err.h>
#include <fcntl.h>
#include "../common/utils.h"

#include "proc_stuff.h"
#include "parser_stuff.h"

void test_fork_execve()
{
	stdfd st = {0,1,2};
	const char** argv = (char**){"ls", "-l", NULL};
	run(argv, &st);
}

void test_parser()
{
	int t;
	char* buf;
	while (1) {
		t = gettoken(&buf);
		switch(t) {
			case T_BAR:
				printf("T_BAR\n");
				break;
			case T_GT:
				printf("T_GT\n");
				break;
			case T_GTGT:
				printf("T_GTGT\n");
				break;
			case T_LT:
				printf("T_LT\n");
				break;
			case T_AMP:
				printf("T_AMP\n");
				break;
			case T_WORD:
				printf("T_WORD %s\n", buf);
				break;
			default:
				printf("END\n");
				return;
		}
	}
}

int main()
{
	printf("Let testing begin!\n");
	/*test_fork_execve();*/
	test_parser();
	printf("Let testing end!\n");
	return 0;
}