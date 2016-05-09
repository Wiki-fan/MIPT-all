#define _POSIX_C_SOURCE 200901L
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <err.h>
#include <termios.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include "game_stuff.h"
#include "config_stuff.h"
#include "tty_stuff.h"
#include "common_types.h"
#include "../common/utils.h"

/* terminal escape symbol */
#define ESC "\033"

static struct termios old_attributes;

timer_t set_timer(struct itimerspec* its)
{
	timer_t timerid;
	struct sigevent sev;
	sigset_t mask;
	/* Block timer signal temporarily */
	printf("Blocking signal %d\n", SIG);
	sigemptyset(&mask);
	sigaddset(&mask, SIG);
	CN1(sigprocmask(SIG_SETMASK, &mask, NULL), E_SIGPROCMASK );

	/* Create the timer */
	sev.sigev_notify = SIGEV_SIGNAL;
	sev.sigev_signo = SIG;
	sev.sigev_value.sival_ptr = &timerid;
	CN1(timer_create(CLOCKID, &sev, &timerid), E_TIMER_CREATE);

	printf("timer ID is 0x%lx\n", (long) timerid);

	/* Start timer */
	CN1( timer_settime( timerid, 0, its, NULL ), E_TIMER_SETTIME );
	printf( "Unblocking signal %d\n", SIG );
	CN1( sigprocmask( SIG_UNBLOCK, &mask, NULL ), E_SIGPROCMASK );

	return timerid;
}

/** Clear screen */
int clear( void )
{
	return printf( ESC "[2J" );
}

/** Set canonical terminal mode */
void set_canonical()
{
	struct termios new_attributes;

	/* Set terminal mode */
	if( !isatty( STDIN_FILENO )) {
		errx( 20, "Input is redirected not from terminal" );
	}

	CN1( tcgetattr( STDIN_FILENO, &old_attributes ), E_TCGETATTR );
	memcpy( &new_attributes, &old_attributes, sizeof( struct termios ));

	/* Disable echoing. */
	new_attributes.c_lflag &= ~ECHO;
	/* Enable non-canonical mode. */
	new_attributes.c_lflag &= ~ICANON;
	/* Minimum number of characters for non-canonical read */
	new_attributes.c_cc[VMIN] = 1;
	/* Strict one byte reading */
	new_attributes.c_cc[VTIME] = 0;

	CN1( tcsetattr( STDIN_FILENO, TCSANOW, &new_attributes ), E_TCSETATTR );
}

/** Restore original terminal mode */
void restore()
{
	CN1( tcsetattr( STDIN_FILENO, TCSANOW, &old_attributes ), E_TCSETATTR );
}

/** Get input from keyboard.
 * If keystroke should be processed, enum ACTION* type is set and return is 0.
 * else return is 1 and enum ACTION* type is undefined */
int get_input( int* type )
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
		case 'f':
			*type = A_MINE;
			fl = 0;
			break;
		case 'e':
			*type = A_USE;
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

/** Render the whole map to the screen */
void render_all()
{
	/* TODO: implement */
}

#undef ESC