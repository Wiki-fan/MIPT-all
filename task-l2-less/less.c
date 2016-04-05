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
#include <wchar.h>
#include <locale.h>
#include "../common/manipulations.h"
#include "../common/utils.h"

#define ADDITIONAL_CHARACTERS_LEN 2
#define LINE_NUMBER_SPECIF "%c%*d:"
#define TAB_WIDTH 4
#define TAB_STR L"    "

struct Screen
{
	wchar_t** strs;
	int* lengths;
	int top, left; /* Top-left coordinates. */
	int terminal_row, terminal_col; /* Terminal window dimensions. */
	int file_col, file_row; /* File dimensions */
	int field_width; /* Working area width */
	int number_width; /* Line number width */
};
typedef struct Screen* pScreen;

/* Prints wide line s to stdout */
void putline( wchar_t* s, int n )
{
	int i = 0;
	while( *s != L'\0' && *s != L'\n' && i < n ) {
		/*putwchar( *s++ );*/
		printf("%lc", *s++);
		++i;
	}
	putwchar( L'\n' );
}

/* Reads wide line from file */
int get_line( FILE* f, wchar_t** str )
{
	int bufSize = 1000; /* Initial buffer size. */
	wint_t c;
	wchar_t* iter;
	int i = 0;
	wchar_t* buf;
	buf = (wchar_t*) malloc_s( bufSize * sizeof( wchar_t ));
	iter = buf;

	while(( c = getwc( f )) != WEOF) {
		if( c == L'\t' ) {
			i += TAB_WIDTH; /* tab is hardcoded 4 whitespaces */
		} else {
			++i;
		}
		/* Realloc if needed. */
		if( i == bufSize - 1 ) {
			bufSize *= 2;
			buf = realloc_s( buf, bufSize * sizeof( wchar_t ));

			iter = buf + i - 1;
		}
		if( c == L'\t' ) {
			swprintf( iter, "%ls", TAB_STR );
			iter += TAB_WIDTH;
		} else {
			*iter++ = c;
		}
		/* Characters that should terminate input. */
		if(/*c == '\0' ||*/ c == L'\n' ) {
			break;
		}
	}
	/* Append '\0' if terminating character was term. */
	if( c != L'\0' ) {
		*iter = L'\0';
	}
	*str = buf;
	return i;
}

/* Special function for less that reads whole file into memory */
void read_less_file( FILE* f, pScreen screen)
{
	wchar_t** ret;
	int* lens;
	int i = 0, j, len;
	int size = 1000; /* Initial string buffer size. */
	ret = (wchar_t**) malloc_s( size * sizeof( wchar_t* ));
	lens = (int*) malloc_s( size * sizeof( int ));

	while(( len = get_line( f, &( ret[i] )))) {
		lens[i] = len;
		++i;
		if( len > screen->file_col) {
			screen->file_col = len;
		}
		if( i > size ) {
			size *= 2;
			ret = realloc_s( ret, size );
			lens = realloc_s( lens, size );
		}
	}
	screen->strs = ret;
	screen->lengths = lens;
	screen->file_row = i;
}

/* Prints output on the screen */
void print_strs( pScreen screen, bool line_print_flag )
{
	int i = screen->file_row;
	int h;


	printf( "\033[H\033[J" ); /* clear screen */
	/*printf("\033[2J\033[1;1H");*/
	/* if file contains less lines than terminal height */
	h = screen->terminal_row > screen->file_row? screen->file_row : screen->terminal_row;
	for( i = 0; i < h; ++i ) {
		if( line_print_flag ) {
			printf( LINE_NUMBER_SPECIF, screen->left>0?'<':'|', screen->number_width, screen->top + i );
		}
		if( screen->left < screen->lengths[screen->top + i] ) {
			putline( screen->strs[screen->top + i] + screen->left, screen->field_width );
		}
		else {
			putchar( '\n' );
		}
	}
}

/* Main less function */
void less( FILE* f, bool line_print_flag )
{
	struct winsize winsz;
	struct Screen screen;
	ioctl( STDIN_FILENO, TIOCGWINSZ, &winsz );
	/*printf( "col %d row %d\n", winsz.ws_col, winsz.ws_row );*/

	int symbol;
	bool fl = true;

	/* set min value to file_col */
	screen.file_col = screen.left = screen.top = 0;
	screen.terminal_row = winsz.ws_row;
	screen.terminal_col = winsz.ws_col;

	read_less_file( f, &screen);
	screen.number_width = 0;
	int i = screen.file_col;
	while (i>0) {
		i /= 10;
		++screen.number_width;
	}
	screen.field_width = line_print_flag ? screen.terminal_col - (ADDITIONAL_CHARACTERS_LEN+screen.number_width) : screen.terminal_col;

	while( fl ) {
		print_strs( &screen, line_print_flag );
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
								screen.top -= screen.top == 0 ? 0 : 1;
								break;
							case 'B': /*down*/
								screen.top += screen.top >= screen.file_row - screen.terminal_row ? 0 : 1;
								break;
							case 'C': /*right*/
								screen.left += screen.left + screen.field_width > screen.file_col ? 0 : 1;
								break;
							case 'D': /*left*/
								screen.left -= screen.left == 0 ? 0 : 1;
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
			case EOF: case 'q':
			case '\004': /* Ctrl+D */
				fl = false;
				break;
			default:;
		}
	}
}


int main( int argc, char* argv[] )
{
	FILE* f;
	int c;

	struct termios old_attributes, new_attributes;
	bool line_print_flag = false;

	/* Should set locale to get wide characters working */
	setlocale(LC_CTYPE, "");
	/*setlocale(LC_ALL, "ru_RU.utf8");*/

	/* Get arguments */
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


	/* Set terminal mode */
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
	/* Strict one byte reading */
	new_attributes.c_cc[VTIME] = 0;

	tcsetattr( STDIN_FILENO, TCSANOW, &new_attributes );

	less( f, line_print_flag );

	tcsetattr( STDIN_FILENO, TCSANOW, &old_attributes );

	return 0;
}