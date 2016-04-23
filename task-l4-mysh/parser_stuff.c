#include "parser_stuff.h"
#include "proc_stuff.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <err.h>
#include <fcntl.h>
#include "../common/utils.h"

typedef enum {
	S_NORMAL, /* */
			S_GT, /* first > seen*/
			S_COMM, /* reading command */
			S_INQUOTES, /* reading quoted string in command */
} STATE;

void push(char* buf, int* buf_size, int* cur_pos, char c)
{
	buf[*cur_pos] = c;
	if (++(*cur_pos) == *buf_size) {
		*buf_size *= 2;
		buf = realloc_s( buf, *buf_size * sizeof( char ));
	}
}

TOKEN gettoken(char** buf)
{
	int state;
	int c;
	int buf_size= 100, cur_pos = 0;
	*buf = (char*) malloc_s( buf_size * sizeof( char ));
	while ( (c = getchar()) != EOF) {
		switch(c) {
			case '|':
				return T_BAR;
			case '>':
				return T_GT;
			case '<':
				if (state != S_GT)
					state = S_GT;
				else return T_GTGT;
			case '&':
				return T_AMP;
			default:
				switch(c) {
					case '|':
					case '>':
					case '<':
					case '&':
					case ' ':
					case '\n':
					case '\t':
						ungetc(c, stdin);
						push(*buf, &buf_size, &cur_pos, '\0');
						return T_WORD;
					default:
						push(*buf, &buf_size, &cur_pos, (char)c);
				}
		}
	}
	return T_END;
}