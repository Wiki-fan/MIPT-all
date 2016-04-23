#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <err.h>
#include <termios.h>
#include <string.h>

/* terminal escape symbol */
#define ESC "\033"
#define CHN1(COMM, CODE, MESSAGE)\
if ((COMM) == -1) \
err(CODE, MESSAGE);

#define CH0(COMM, CODE, MESSAGE)\
if ((COMM) == 0) \
err(CODE, MESSAGE);


static struct termios old_attributes;

int clear(void)
{
	return printf(ESC "[2J");
}

int make_canon()
{
	struct termios new_attributes;

	/* Set terminal mode */
	if( !isatty( STDIN_FILENO ))
	{
		errx( 20, "Input is redirected not from terminal" );
	}

	CHN1(tcgetattr( STDIN_FILENO, &old_attributes ), 21, "Can't get terminal state");
	memcpy( &new_attributes, &old_attributes, sizeof( struct termios ));

	/* Disable echoing. */
	new_attributes.c_lflag &= ~ECHO;
	/* Enable non-canonical mode. */
	new_attributes.c_lflag &= ~ICANON;
	/* Minimum number of characters for non-canonical read */
	new_attributes.c_cc[VMIN] = 1;
	/* Strict one byte reading */
	new_attributes.c_cc[VTIME] = 0;

	CHN1(tcsetattr( STDIN_FILENO, TCSANOW, &new_attributes ), 22, "Can't set new terminal state");
}

int restore()
{
	CHN1(tcsetattr( STDIN_FILENO, TCSANOW, &old_attributes ) == -1, 20, "Can't restore original terminal state");
}
