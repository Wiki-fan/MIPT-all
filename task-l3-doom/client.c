#define _POSIX_C_SOURCE 199309L
#include <signal.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <err.h>
#include <strings.h>
#include <unistd.h>
#include "../common/utils.h"
#include "game_stuff.h"
#include "tty_stuff.h"
#include "config_stuff.h"
#include "net_stuff.h"
#include "common_types.h"
#include "client_stuff.h"

volatile int cnt; /*for debug */
volatile int action;

/* Global timer ID */
timer_t timerid;
struct itimerspec its;

enum GAME {
	G_CONTINUE, G_RESTART, G_EXIT, G_CHANGE_ROOM, G_WAIT_END
};
volatile int game;

/**Asks server for action and retrieves response. Sets global flag "game".
 * Uses unsafe printf function, because when it is actually using SIGALRM can not terminate any other unsafe function.
 */
void process_recv(int act)
{
	int i;
	/*printf( "Got input %d\n", act );*/
	send_int( act, sockfd );
	blocking_read_int( sockfd, &i );
	switch( i ) {
		case R_ROOM_CLOSED:
			printf( "Host closed room\n" );
			stop_timer(timerid);
			game = G_CHANGE_ROOM;
			break;
		case R_GAME_STOPPED:
			printf( "Game stopped\n" );
			stop_timer(timerid);
			game = G_RESTART;
			break;
		case R_GAME_FINISHED:
			printf( "##########################\n"
					"#You win, congratulations#\n"
					"##########################\n");
			game = G_RESTART;
			break;
		case R_DIED:
			printf( "##########\n"
			        "#YOU DIED#\n"
					"##########\n");
			stop_timer(timerid);
			game = G_WAIT_END;
			break;
		case R_DONE:
			render();
			game = G_CONTINUE;
			break;
		default:
			errx( 13, "Wrong response" );
	}
}

/** We grab first action from keyboard and send it to server as soon as we can */
static void handler(int signum)
{
	if (action != -1) {
		process_recv( action );
	} else {
		process_recv( A_NONE);
	}
	++cnt;
	/*printf("\n%d step", cnt);*/
	action = -1;
}

void input_loop()
{
	/* if we don't have unprocessed actions */
	if( action == -1 ) {
		get_input( &action );
		/* If we should exit game because user said so */
		if( action == A_EXIT ) {
			block_timer_signal();
			game = G_EXIT;
		}
	}
	if (game == G_WAIT_END) {
		int response;

		printf("Waiting till game ends.\n");
		blocking_read_int(sockfd, &response);
		switch(response) {
			case R_GAME_FINISHED:
				printf("Game finished\n");
				game = G_RESTART;
			case R_GAME_STOPPED:
				game = G_RESTART;
				break;
			case R_ROOM_CLOSED:
				game = G_CHANGE_ROOM;
				break;
			default:
				errx(3, "Wrong parameter");
		}
	}
}

void game_round()
{
	printf( "Waiting for game to start\n" );
	CHK_RESPONSE( R_GAME_STARTED, "Game started" );

	set_canonical();
	start_timer( timerid, &its );
	process_recv( A_NONE );

	while( !(game == G_RESTART || game == G_CHANGE_ROOM || game == G_EXIT) ) {
		input_loop();
	}

	/* Restore original terminal state */
	restore();
}

void choose_room_and_play()
{
	int i;
	float step_standard_delay;
	/* Receive rooms list  */
	send_int( A_ASK_ROOMS_LIST, sockfd );
	CHK_RESPONSE( R_SENDING_ROOMS, "List of rooms received" );
	printf( "Rooms:\n" );
	i = ask_which_room( getrecvlist());

	ask_player_name( buf+sizeof(int) );
	send_int( A_JOIN_ROOM, sockfd );

	memcpy(buf, &i, sizeof(int));
	send_buf( sockfd, MAX_NAME_LEN+sizeof(int), buf );
	CHK_RESPONSE( R_JOINED, "You joined" );
	blocking_read_buf(sockfd, (char*)&step_standard_delay);

	/* Initialize timer values */
	its.it_value.tv_sec = 0;
	its.it_value.tv_nsec = step_standard_delay*NSEC_IN_SEC;
	its.it_interval.tv_sec = 0;
	its.it_interval.tv_nsec = step_standard_delay*NSEC_IN_SEC;

	while ( !( game == G_CHANGE_ROOM || game == G_EXIT) ) {
		game_round();
	}
}

void play()
{
	struct sigaction sa;
	action = -1;
	cnt = 0;

	timerid = create_timer();

	/* Setup handler */
	sa.sa_handler = handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags =0 /*SA_RESTART*/;
	CN1(sigaction(SIG, &sa, NULL), E_SIGACTION);

	while(game != G_EXIT) { /* G_CHANGE_ROOM */
		choose_room_and_play();
	}
}

void host_input_loop()
{
	switch( ask_host_action()) {
		case A_START_GAME:
			send_int( A_START_GAME, sockfd );
			CHK_RESPONSE( R_GAME_STARTED, "Game started" );
			break;
		case A_STOP_GAME:
			send_int( A_STOP_GAME, sockfd );
			CHK_RESPONSE( R_GAME_STOPPED, "Game stopped" );
			break;
		case A_CLOSE_ROOM:
			send_int( A_CLOSE_ROOM, sockfd );
			CHK_RESPONSE( R_ROOM_CLOSED, "Room closed" );
			game = G_CHANGE_ROOM;
			break;
		case A_ASK_PLAYER_LIST:
			send_int( A_ASK_PLAYER_LIST, sockfd );
			CHK_RESPONSE( R_SENDING_PLAYERS, "Receiving players list" );
			printf( "Players:\n" );
			getplayerlist();
			break;
		case A_EXIT:
			exit( 0 );
		default:
			errx( 3, "Unreachable code" );
	}
}

int host()
{
	/* Create room */
	while(!(game == G_EXIT)) {
		ask_room_name( buf );
		send_int( A_CREATE_ROOM, sockfd );
		send_buf( sockfd, MAX_NAME_LEN, buf );
		CHK_RESPONSE( R_ROOM_CREATED, "Room is created on the server" );

		/* Possible actions of host */
		while( !(game == G_EXIT || game == G_CHANGE_ROOM) ) {
			host_input_loop();
		}
	}
}



int main()
{
	/* Determining role of client */
	int act = ask_player_or_host();

	/* Connect */
	sockfd = setup_connection();
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
