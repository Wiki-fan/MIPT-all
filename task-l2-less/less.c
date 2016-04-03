#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <err.h>
#include <getopt.h>
#include <slcurses.h>
#include <stdbool.h>
#include <sys/ioctl.h>
#include "../common/manipulations.h"
#include "../common/utils.h"

#define LINE_NUMBER_LEN 5
#define LINE_NUMBER_SPECIF "%3d: "
#define TAB_WIDTH 4
#define TAB_STR "    "

struct Cursor
{
	int top, left; /* Top-left coordinates. */
	int terminal_row, terminal_col; /* Terminal window dimensions. */
	int file_col, file_row; /* File dimensions */
};
typedef struct Cursor* pCursor;

void putline( char* s, int n )
{
	int i = 0;
	while( *s != '\0' && *s != '\n' && i < n ) {
		putchar( *s++ );
		++i;
	}
	putchar( '\n' );
}

int get_line( FILE* f, char** str )
{
	int bufSize = 1000; /* Initial buffer size. */
	int c;
	char* iter;
	int i = 0;
	char* buf;
	buf = (char*) malloc_s( bufSize * sizeof( char ));
	iter = buf;

	while(( c = getc( f )) != EOF) {
		if( c == '\t' ) {
			i += TAB_WIDTH; /* tab is hardcoded 4 whitespaces */
		} else {
			++i;
		}
		/* Realloc if needed. */
		if( i == bufSize - 1 ) {
			bufSize *= 2;
			buf = realloc_s( buf, bufSize * sizeof( char ));

			iter = buf + i - 1;
		}
		if( c == '\t' ) {
			sprintf( iter, "%s", TAB_STR );
			iter += TAB_WIDTH;
		} else {
			*iter++ = c;
		}
		/* Characters that should terminate input. */
		if(/*c == '\0' ||*/ c == '\n' ) {
			break;
		}
	}
	/* Append '\0' if terminating character was term. */
	if( c != '\0' ) {
		*iter = '\0';
	}
	*str = buf;
	return i;
}

void read_file( FILE* f, char*** strs, int** lengths, int* lines_number, int* columns_number )
{
	char** ret;
	int* lens;
	int i = 0, j, len;
	int size = 1000; /* Initial string buffer size. */
	ret = (char**) malloc_s( size * sizeof( char* ));
	lens = (int*) malloc_s( size * sizeof( int ));

	while(( len = get_line( f, &( ret[i] )))) {
		lens[i] = len;
		++i;
		if( len > *columns_number ) {
			*columns_number = len;
		}
		if( i > size ) {
			size *= 2;
			ret = realloc_s( ret, size );
			lens = realloc_s( lens, size );
		}
	}
	*strs = ret;
	*lengths = lens;
	*lines_number = i;
}


void print_strs( char** strs, int* lengths, pCursor cursor, bool line_print_flag )
{
	int i;
	int h;
	int width = line_print_flag ? cursor->terminal_col - LINE_NUMBER_LEN : cursor->terminal_col;
	printf( "\033[H\033[J" ); /* clear screen */
	/* if file contains less lines than terminal height */
	h = cursor->terminal_row > cursor->file_row? cursor->file_row : cursor->terminal_row;
	for( i = 0; i < h; ++i ) {
		if( line_print_flag ) {
			printf( LINE_NUMBER_SPECIF, cursor->top + i );
		}
		if( cursor->left < lengths[cursor->top + i] ) {
			putline( strs[cursor->top + i] + cursor->left, width );
		}
		else {
			putchar( '\n' );
		}
	}
}

void less( FILE* f, bool line_print_flag )
{
	struct winsize winsz;
	struct Cursor cursor;
	ioctl( STDIN_FILENO, TIOCGWINSZ, &winsz );
	printf( "col %d row %d\n", winsz.ws_col, winsz.ws_row );

	char** strs;
	int* lengths;
	int symbol;
	bool fl = true;

	read_file( f, &strs, &lengths, &cursor.file_row, &cursor.file_col );
	/*printf( "%d %d\n", cursor.file_row, cursor.file_col );*/

	cursor.left = cursor.top = 0;
	cursor.file_col = winsz.ws_col;
	cursor.terminal_row = winsz.ws_row;
	cursor.terminal_col = winsz.ws_col;

	while( fl ) {
		/*printf( "Был введён символ: код=%d начертание '%c'\n", symbol, (char) symbol );*/
		print_strs( strs, lengths, &cursor, line_print_flag );
		symbol = getchar();
		switch( symbol ) {
			case 27:
				symbol = getchar();
				switch( symbol ) {
					case 91:
						symbol = getchar();
						switch( symbol ) {
							case EOF:
								break;
							case 'A': /*up*/
								cursor.top -= cursor.top == 0 ? 0 : 1;
								break;
							case 'B': /*down*/
								cursor.top += cursor.top == cursor.file_row - cursor.terminal_row ? 0 : 1;
								break;
							case 'C': /*right*/
								cursor.left += /*cursor.left - winsz.ws_col < columns_number ? 0 :*/ 1;
								break;
							case 'D': /*left*/
								cursor.left -= cursor.left == 0 ? 0 : 1;
								break;
							default:;
						}
						break;
					case EOF:
						fl = false;
						break;
					default:;
				}
				break;
			case EOF:
				fl = false;
				break;
			default:;
		}
		/*printf( "%d %d \n", top, left );*/
		if( symbol == '\004' ) {
			break;
		}
	}
}


int main( int argc, char* argv[] )
{
	FILE* f;
	int c;

	struct termios old_attributes, new_attributes;
	bool line_print_flag = false;

	while(( c = getopt( argc, argv, "n" )) != -1 ) {
		switch( c ) {
			case 'n':
				line_print_flag = true;
				break;
			case '?':
				errx( 11, "Unknown parameter" );
			default:
				errx( 3, "Unreachable code" );
		}
	}

	if( argc == 1 ) {
		f = stdin;
	}
	else {
		if( optind >= argc ) {
			errx( 12, "Expected argument after options\n" );
		}
		f = fopen_s( argv[optind], "r" );
	}

	if( !isatty( STDIN_FILENO )) {
		errx( 20, "Input is redirected not from terminal. Error" );
	}

	tcgetattr( STDIN_FILENO, &old_attributes );
	memcpy( &new_attributes, &old_attributes, sizeof( struct termios ));

	/* Disable echoing. */
	new_attributes.c_lflag &= ~ECHO;
	/* Enable non-canonical mode. */
	new_attributes.c_lflag &= ~ICANON;
	/* Minimum number of characters for non-canonical read */
	new_attributes.c_cc[VMIN] = 1;

	/* Включает жёсткое чтение по одному символу.
	 * Терминал будет работать не как сокет.
	 * Поскольку как-то оно слишком агрессивно, по моему
	 * мнению я это выключу нафик.
	 */
	/*
	new_attributes.c_cc[VTIME] = 0;
	*/
	tcsetattr( STDIN_FILENO, TCSANOW, &new_attributes );

	less( f, line_print_flag );

	tcsetattr( STDIN_FILENO, TCSANOW, &old_attributes );

	return 0;
}