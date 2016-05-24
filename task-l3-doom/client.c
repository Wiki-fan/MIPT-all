#define _POSIX_C_SOURCE 200901L
#include <signal.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <err.h>
#include <errno.h>
#include <poll.h>
#include "../common/utils.h"
#include "game_stuff.h"
#include "client_action_stuff.h"

/*for debug */
volatile int cnt;

volatile int action;
extern struct pollfd poll_arr[NUMPOLLS];

volatile int game;

int main()
{
	/* Determining role of client */
	int act = ask_player_or_host();

	game = G_CONTINUE;

	/* Connect */
	setup_connection();
	printf( "Connection established\n" );

	switch( act ) {
		case A_CREATE_ROOM:
			host();
			break;
		case A_JOIN_ROOM:
			play();
			break;
		case A_EXIT:
			return 0;
		default:
			errx( 3, "Unreachable code" );
	}
	return 0;
}
