#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <err.h>
#include <termios.h>
#include <string.h>
#include "game_stuff.h"
#include "config_stuff.h"
extern Player player;
extern Map map;

/* terminal escape symbol */
#define ESC "\033"
#define CHN1( COMM, CODE, MESSAGE )\
if ((COMM) == -1) \
err(CODE, MESSAGE);

#define CH0( COMM, CODE, MESSAGE )\
if ((COMM) == 0) \
err(CODE, MESSAGE);


static struct termios old_attributes;

int clear( void )
{
	return printf( ESC "[2J" );
}

int set_canonical()
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

int restore()
{
	CHN1( tcsetattr( STDIN_FILENO, TCSANOW, &old_attributes ) == -1, 20, "Can't restore original terminal state" );
}

int get_input()
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
							move( 0, -1 );
							break;
						case 'B': /*down*/
							move( 0, 1 );
							break;
						case 'C': /*right*/
							move( 1, 0 );
							break;
						case 'D': /*left*/
							move( -1, 0 );
							break;
						default:;
					}
					break;
				case EOF:
					fl = 0;
					break;
				default:;
			}
			break;
		case EOF:
		case 'q':
		case '\004': /* Ctrl+D */
			fl = 0;
			break;
		case 'w': /*up*/
			move( 0, -1 );
			break;
		case 's': /*down*/
			move( 0, 1 );
			break;
		case 'd': /*right*/
			move( 1, 0 );
			break;
		case 'a': /*left*/
			move( -1, 0 );
			break;
		case 'm':
			player_set_mine();
			break;
		default:;
	}
	return fl;
}

#define FIELD_OF_SIGHT 10
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