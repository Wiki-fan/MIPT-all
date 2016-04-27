#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <err.h>
#include <fcntl.h>
#include "../common/utils.h"
#include "proc_stuff.h"
#include "parser_stuff.h"

int main()
{
	TOKEN token = T_NL;
	while( 1 ) {
		if( token == T_NL ) {
			printf( PROMPT );
		}
		token = process( 0, 0 );
		if (token == T_END)
			break;
	}
	return 0;
}