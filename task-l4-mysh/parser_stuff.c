#include "parser_stuff.h"
#include "proc_stuff.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <err.h>
#include "../common/utils.h"
#include "error_stuff.h"

/**S_NORMAL state is reading next token
 * S_GT state is reading << or < token
 * S_INWORD state is reading word (file name or command)
 * S_INQUOTES state is reading quoted string
 */
typedef enum {
	S_NORMAL, /* getting token */
	S_GT, /* first > seen*/
	S_INWORD, /* reading word (part of command) */
	S_INQUOTES /* reading quoted string in command */
} STATE;

/** Appends character to buffer */
void push( char* buf, int* buf_size, int* cur_pos, char c )
{
	buf[*cur_pos] = c;
	if( ++( *cur_pos ) == *buf_size ) {
		*buf_size *= 2;
		buf = realloc_s( buf, *buf_size * sizeof( char ));
	}
}

/** Gets next token from input stream */
TOKEN gettoken( char** buf )
{
	int state = S_NORMAL;
	int c;
	int buf_size = 100, cur_pos = 0;
	*buf = (char*) malloc_s( buf_size * sizeof( char ));
	while(( c = getchar()) != EOF) {
		/* endline backslash processing */
		if (c =='\\') {
			/* if after \ goes \n, skip both, if other char, push \ and this char */
			if(( c = getchar()) != '\n' ) {
				push( *buf, &buf_size, &cur_pos, '\\' );
				push( *buf, &buf_size, &cur_pos, (char) c );
			}
			continue;
		}

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
					case '"':
						state = S_INQUOTES;
						continue;
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
			case S_INQUOTES:
				switch( c ) {
					case '"':
						push( *buf, &buf_size, &cur_pos, '\0' );
						return T_WORD;
					default:
						push( *buf, &buf_size, &cur_pos, (char) c );
						continue;
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
				RAISEX(E_UNREACHABLE);
		}
	}
	return T_END;
}

/**Processes current command.
 * If makepipe, this process should get input from other by pipe. Pipe fd is returned in pipefd.
 */
TOKEN process( int* retpid, int makepipe, int* pipefd )
{
	/* variadic size buffer for input commands */
	char* str;
	/* initial size of argv for child process */
	int argv_sz = 2;
	TOKEN token, term;
	int pid;
	/* file descriptors for pipe (if needed) */
	int pfd[2] = { -1, -1 };
	proc pr;
	int i;
	pr.in = STDIN_FILENO;
	pr.out = STDOUT_FILENO;
	pr.argc = 0;
	pr.argv = malloc_s( argv_sz * sizeof( char* ));
	pr.infile = pr.outfile = NULL;
	pr.append = 0;
	pr.backgr = 0;

	/* non-zero in file name means that stream must be redirected */
	while( 1 ) {
		switch( token = gettoken( &str )) {
			case T_WORD:
				pr.argv[pr.argc] = str;
				++pr.argc;
				if( pr.argc >= argv_sz ) {
					argv_sz *= 2;
					pr.argv = realloc_s( pr.argv, argv_sz * sizeof( char* ));
				}
				break;
			case T_LT:
				/* if input redirect already set by | or < operator */
				if( pr.in != STDIN_FILENO || pr.infile != 0) {
					DROP( "Extra <" );
				}
				if( gettoken( &pr.infile ) != T_WORD ) {
					DROP( "No file specified after <" );
				}
				break;
			case T_GT:
			case T_GTGT:
				/* if output redirect was already set by > or >> operator */
				if( pr.out != STDOUT_FILENO || pr.outfile != 0 ) {
					DROP( "Extra > or >>" );
				}
				if( gettoken( &pr.outfile ) != T_WORD ) {
					DROP( "No file specified after > or >>" );
				}
				if( token == T_GTGT ) {
					pr.append = 1;
				}
				break;
			case T_BAR:
			case T_AMP:
			case T_NL:
				pr.argv[pr.argc] = NULL;
				/* redirect pipes if needed */
				if( token == T_BAR ) {
					if( pr.out != STDOUT_FILENO || pr.outfile != 0 ) {
						DROP( "| is conflicting with > or >>" );
					}
					/* running next command and telling it that it should make pipe */
					term = process( retpid, 1, &pr.out );
				} else {
					term = token;
				}

				/* Creating pipe */
				if( makepipe ) {
					CN1( pipe( pfd ), E_PIPE );
					*pipefd = pfd[1];
					pr.in = pfd[0];
				}

				/* Checking if should run in background */
				pr.backgr = (term == T_AMP);
				CN1( pid = run( &pr, pfd[1] ), E_RUN );

				/* pid of process to wait */
				if( token != T_BAR ) {
					*retpid = pid;
				}

				/* freeing argv */
				for( i = 0; i < pr.argc; ++i ) {
					free( pr.argv[i] );
				}
				free( pr.argv );
				return term;
			case T_ERROR:
				while (token = gettoken( &str )) {
					if (token == T_BAR || token == T_AMP || token == T_WORD) {
						break;
					}
				}
			default:
				errx( 3, "Unreachable code" );
		}
	}
}