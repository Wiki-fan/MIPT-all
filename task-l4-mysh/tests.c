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
	proc st = { 0, 1, 2 };
	char* argv[3];
	argv[0] = "/bin/ls";
	argv[1] = "-la";
	argv[3] = NULL;
	run( argv, &st );
}

TOKEN test_gettoken()
{
	TOKEN t;
	char* buf;

	t = gettoken( &buf );
	switch( t ) {
		case T_BAR:
			printf( "T_BAR\n" );
			break;
		case T_GT:
			printf( "T_GT\n" );
			break;
		case T_GTGT:
			printf( "T_GTGT\n" );
			break;
		case T_LT:
			printf( "T_LT\n" );
			break;
		case T_AMP:
			printf( "T_AMP\n" );
			break;
		case T_WORD:
			printf( "T_WORD %s\n", buf );
			break;
		case T_NL:
			printf( "T_NL\n");
			break;
		case T_END:
			printf( "END\n" );
			break;
		default:
			errx(30, "Missed token");
	}
	free( buf );
	return t;
}

#define PROMPT ">"
void test_prompt()
{
	TOKEN token = T_NL;
	while( 1 ) {
		if( token == T_NL ) {
			printf( PROMPT );
		}
		token = test_gettoken();
		if (token == T_END)
			break;
	}
}
int main()
{
	/*test_fork_execve();*/
	test_prompt();
	return 0;
}