#include "parser_stuff.h"
#include "proc_stuff.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <err.h>
#include <fcntl.h>
#include "../common/utils.h"

/*
 * S_NORMAL state is reading next token
 * S_GT state is reading << or < token
 * S_INWORD state is reading word (file name or command)
 * S_INWORD state is reading quoted string
 */
typedef enum {
	S_NORMAL, /* */
			S_GT, /* first > seen*/
			S_INWORD, /* reading command */
			S_INQUOTES, /* reading quoted string in command */
} STATE;

/* appends character to buffer */
void push( char* buf, int* buf_size, int* cur_pos, char c )
{
	buf[*cur_pos] = c;
	if( ++( *cur_pos ) == *buf_size ) {
		*buf_size *= 2;
		buf = realloc_s( buf, *buf_size * sizeof( char ));
	}
}

TOKEN gettoken( char** buf )
{
	int state = S_NORMAL;
	int c;
	int buf_size = 100, cur_pos = 0;
	*buf = (char*) malloc_s( buf_size * sizeof( char ));
	while(( c = getchar()) != EOF) {
		switch( state ) {
			case S_NORMAL:
				switch( c ) {
					case ' ':
					case '\t':
						continue;
					case '\n':
						return T_NL;
					case '|':
						return T_BAR;
					case '>':
						state = S_GT;
						continue;
					case '<':
						return T_LT;
					case '&':
						return T_AMP;
					default:
						state = S_INWORD;
						push( *buf, &buf_size, &cur_pos, (char) c );
						continue;
				}
			case S_GT:
				if( c == '>' ) {
					return T_GTGT;
				} else {
					ungetc( c, stdin );
					return T_GT;
				}
			case S_INWORD:
				switch( c ) {
					case '|':
					case '>':
					case '<':
					case '&':
					case ' ':
					case '\n':
					case '\t':
						ungetc( c, stdin );
						push( *buf, &buf_size, &cur_pos, '\0' );
						return T_WORD;
					default:
						push( *buf, &buf_size, &cur_pos, (char) c );
				}
				continue;
			default:
				errx(3, "Unreachable code");
		}
	}
	return T_END;
}

int process()
{
	char* str;

	int append = 0;
	int argv_sz = 2;
	int token;
	proc pr;
	pr.in = STDIN_FILENO;
	pr.out = STDOUT_FILENO;
	pr.argc = 0;
	*pr.argv = malloc_s(argv_sz*sizeof(char*));

	/* -1 in descriptor means that stream must be redirected */
	while (1) {
		switch(token = gettoken(&str)) {
			case T_WORD:
				pr.argv[0][pr.argc] = str;
				++pr.argc;
				if (pr.argc >= argv_sz) {
					argv_sz *= 2;
					*pr.argv = realloc_s(pr.argv, argv_sz*sizeof(char*));
				}
				break;
			case T_LT:
				if (gettoken(&pr.infile) != T_WORD) {
					errx(26, "No file specified after <");
				}
				pr.in = -1;
				break;
			case T_GT:
			case T_GTGT:
				/* if output redirect was already set */
				if (pr.out != STDOUT_FILENO) {
					errx(27, "Extra > or >>");
				}
				if (gettoken(&pr.outfile) != T_WORD) {
					errx(26, "No word specified after > or >>");
				}
				pr.out = -1;
				if (token == T_GTGT) {
					append = 1;
				}
				break;
			case T_BAR:
				pr.argv[pr.argc] = NULL;
				if (token == T_BAR) {
					if (pr.out != STDOUT_FILENO) {
						errx(28, "| is conflicting with > or >>");
					}
					/* COMMAND */
				}

		}
	}
}