#define _POSIX_C_SOURCE 200901L
#include <signal.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <err.h>

#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <fcntl.h>

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

enum GAME {
	G_CONTINUE, G_RESTART, G_EXIT
};
volatile int game;

/* We grab first action from keyboard and send it to server as soon as we can */
static void handler(int signum)
{
	/*printf("Timer tick\n");*/
	if (action != -1) {
		process_recv( action );
	} else {
	/*	process_recv( A_NONE);*/
	}
	++cnt;
	/*printf("\n%d step", cnt);*/
	action = -1;
}

#define SIG SIGALRM
#define CLOCKID CLOCK_REALTIME
#define TIMER_MS_DELAY 100
#define TIMER_NS_DELAY TIMER_MS_DELAY * 1000000

/* Asks server for action and retrieves response. Sets global flag "game".
 * Uses unsafe printf function, because SIGALRM can not terminate any other unsafe function. */
int process_recv(int act)
{
	int i;
	/*printf( "Got input %d\n", act );*/
	send_int( act, sockfd );
	i = read_int( sockfd );
	switch( i ) {
		case R_ROOM_CLOSED:
			printf( "Host closed room\n" );
			game = G_RESTART;
			return G_RESTART;
		case R_GAME_STOPPED:
			printf( "Game finished\n" );
			game = G_RESTART;
			return G_RESTART;
		case R_DIED:
			printf( "YOU DIED!!!!\n\n" );
			game = G_RESTART;
			return G_RESTART;
		case R_DONE:
			render();
			game = G_CONTINUE;
			return G_CONTINUE;
		default:
			errx( 13, "Wrong response" );
	}
}

void play()
{
	int i;
	struct sigaction sa;
	timer_t timerid;
	struct sigevent sev;
	struct itimerspec its;
	sigset_t mask;
	int fl = 1;
	action = -1;

	cnt = 0;
	sa.sa_handler = handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	CHN1(sigaction(SIG, &sa, NULL), 33, "Error setting sigaction");

	/* Block timer signal temporarily */
	printf("Blocking signal %d\n", SIG);
	sigemptyset(&mask);
	sigaddset(&mask, SIG);
	CHN1(sigprocmask(SIG_SETMASK, &mask, NULL), 34, "sigprocmask failed");

	/* Create the timer */
	sev.sigev_notify = SIGEV_SIGNAL;
	sev.sigev_signo = SIG;
	sev.sigev_value.sival_ptr = &timerid;
	CHN1(timer_create(CLOCKID, &sev, &timerid), 35, "timer_create failed");

	printf("timer ID is 0x%lx\n", (long) timerid);

	/* Initialize timer values */
	its.it_value.tv_sec = 0;
	its.it_value.tv_nsec = TIMER_NS_DELAY;
	its.it_interval.tv_sec = 0;
	its.it_interval.tv_nsec = TIMER_NS_DELAY;


	while(1) {
		/* Receive rooms list  */
		send_int( A_ASK_ROOMS_LIST, sockfd );
		CHK_RESPONSE( R_SENDING_ROOMS, "List of rooms received" );
		printf( "Rooms:\n" );
		i = ask_which_room( getrecvlist());

		ask_player_name( buf );
		send_int( A_JOINROOM, sockfd );
		send_int( i, sockfd );
		send_buf( sockfd, MAX_NAME_LEN, buf );
		CHK_RESPONSE( R_JOINED, "You joined" );

		printf( "Waiting for game to start\n" );
		CHK_RESPONSE( R_GAME_STARTED, "Game started" );
		/*CHK_RESPONSE( R_DONE, "Game info sended" );*/

		set_canonical();
		process_recv( A_NONE );

		/* Start timer */
		CHN1( timer_settime( timerid, 0, &its, NULL ), 36, "timer_settime failed" );
		printf( "Unblocking signal %d\n", SIG );
		CHN1( sigprocmask( SIG_UNBLOCK, &mask, NULL ), 37, "sigprocmask failed" );
		while( 1 ) {
			int is_valid;

			if( action == -1 ) {
				get_input( &action );
				/* If we should exit game because user said so */
				if( action == A_EXIT ) {
					fl = 0;
					break;
				}
			} else {
				/*wait?*/
				continue;
			}

			/* If we should run our game one more time */
			if( game == G_RESTART ) {
				fl = 1;
				break;
			}

		}
		/* Stop timer */
		CHN1( timer_settime( timerid, 0, 0, NULL ), 36, "timer_settime failed" );
		/* Restore original terminal state */
		restore();

		if (fl == 0) {
			break; /* exit game loop */
		}
	}
}

int host()
{
	/* Create room */
	ask_room_name( buf );
	send_int( A_CREATE_ROOM, sockfd );
	send_buf( sockfd, MAX_NAME_LEN, buf );
	CHK_RESPONSE( R_ROOM_CREATED, "Room is created on the server" );

	/* Possible actions of host */
	while( 1 ) {
		switch( ask_host_action()) {
			case A_START_GAME:
				send_int( A_START_GAME, sockfd );
				CHK_RESPONSE( R_GAME_STARTED, "Game started" );
				break;
			case A_STOP_GAME:
				send_int( A_START_GAME, sockfd );
				CHK_RESPONSE( R_GAME_STOPPED, "Game started" );
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
		case A_JOINROOM:
			play();
			break;
		case A_EXIT:
			return 0;
		default:
			errx( 3, "Unreachable code" );
	}
	return 0;
}
