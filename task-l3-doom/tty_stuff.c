#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <err.h>
#include <termios.h>
#include <string.h>
#include "game_stuff.h"
#include "config_stuff.h"
#include "tty_stuff.h"
#include "common_types.h"
#include "../common/utils.h"

extern Player player;
extern Map map;

/* terminal escape symbol */
#define ESC "\033"

static struct termios old_attributes;

/** clear screen */
int clear( void )
{
	return printf( ESC "[2J" );
}

/** set canonical terminal mode */
void set_canonical()
{
	struct termios new_attributes;

	/* Set terminal mode */
	if( !isatty( STDIN_FILENO )) {
		errx( 20, "Input is redirected not from terminal" );
	}

	CHN1( tcgetattr( STDIN_FILENO, &old_attributes ), 21, "Can't get terminal state" );
	memcpy( &new_attributes, &old_attributes, sizeof( struct termios ));

	/* Disable echoing. */
	new_attributes.c_lflag &= ~ECHO;
	/* Enable non-canonical mode. */
	new_attributes.c_lflag &= ~ICANON;
	/* Minimum number of characters for non-canonical read */
	new_attributes.c_cc[VMIN] = 1;
	/* Strict one byte reading */
	new_attributes.c_cc[VTIME] = 0;

	CHN1( tcsetattr( STDIN_FILENO, TCSANOW, &new_attributes ), 22, "Can't set new terminal state" );
}

/** restore original terminal mode */
void restore()
{
	CHN1( tcsetattr( STDIN_FILENO, TCSANOW, &old_attributes ), 20, "Can't restore original terminal state" );
}

/** Gets input from keyboard.
 * If keystroke should be processed, enum ACTION* type is set and return is 0.
 * else return is 1 and enum ACTION* type is undefined */
int get_input( enum ACTION* type )
{
	int fl = 1;
	int symbol;
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
							*type = A_UP;
							fl = 0;
							break;
						case 'B': /*down*/
							*type = A_DOWN;
							fl = 0;
							break;
						case 'C': /*right*/
							*type = A_RIGHT;
							fl = 0;
							break;
						case 'D': /*left*/
							*type = A_LEFT;
							fl = 0;
							break;
						default:;
					}
					break;
				case EOF:
					/*fl = 0;*/
					break;
				default:;
			}
			break;
		case EOF:
		case 'q':
		case '\004': /* Ctrl+D */
			*type = A_EXIT;
			fl = 0;
			break;
		case 'w': /*up*/
			*type = A_UP;
			fl = 0;
			break;
		case 's': /*down*/
			*type = A_DOWN;
			fl = 0;
			break;
		case 'd': /*right*/
			*type = A_RIGHT;
			fl = 0;
			break;
		case 'a': /*left*/
			*type = A_LEFT;
			fl = 0;
			break;
		case 'm': case 'e':
			*type = A_MINE;
			fl = 0;
			break;
		case ' ':
			*type = A_ATTACK;
			fl = 0;
			break;
		default:;
	}
	return fl;
}

void render()
{
	int x, y;
	clear();
	for( y = player.y - FIELD_OF_SIGHT + 1; y < player.y + FIELD_OF_SIGHT; ++y ) {
		for( x = player.x - FIELD_OF_SIGHT + 1; x < player.x + FIELD_OF_SIGHT; ++x ) {
			if( x >= 0 && x < map.w && y >= 0 && y < map.h ) {
				if( player.x == x && player.y == y ) {
					putchar( PLAYER );
				} else {
					putchar( map.m[y][x] );
				}
			} else {
				putchar( ' ' );
			}
		}
		putchar( '\n' );
	}
}

void render_all()
{

}

#undef ESC