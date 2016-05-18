#include "client_stuff.h"
#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <time.h>
#include <poll.h>
#include <strings.h>
#include "../common/utils.h"
#include "tty_stuff.h"
#include "net_stuff.h"

struct pollfd poll_arr[NUMPOLLS];

char buf[CLIENT_BUF_SIZE];
Player player;

/* Initialize poll_arr */
void setup_connection()
{
	int portno;
	struct sockaddr_in serv_addr;
	struct hostent* server;

	portno = PORT;

	CN1( poll_arr[P_SOCK].fd = socket( AF_INET, SOCK_STREAM, 0 ), E_SOCKET );
	poll_arr[P_STDIN].fd = STDIN_FILENO;

	CH0( server = gethostbyname( HOSTNAME ), 25, "No such hostname" );

	bzero((char*) &serv_addr, sizeof( serv_addr ));
	serv_addr.sin_family = AF_INET;
	/* TODO: macros expansion in macros. h_addr don't work */
	bcopy((char*) server->h_addr_list[0], (char*) &serv_addr.sin_addr.s_addr, server->h_length );
	serv_addr.sin_port = htons( (uint16_t)portno );

	/* Now connect to the server */
	CN1( connect( poll_arr[P_SOCK].fd, (struct sockaddr*) &serv_addr, sizeof( serv_addr )), E_CONNECT );
	poll_arr[P_SOCK].events = POLLIN | POLLERR | POLLHUP;
	poll_arr[P_STDIN].events = POLLIN | POLLERR | POLLHUP;
}

int ask_player_or_host()
{
	int response;

	while( 1 ) {
		printf( "What do you want?\n1. Create room on the server.\n2. Play in already created room.\n3. Exit.\n" );
		scanf( "%d", &response );
		switch( response ) {
			case 1:
				return A_CREATE_ROOM;
			case 2:
				return A_JOIN_ROOM;
			case 3:
				return A_EXIT;
			default:
				break;
		}
		printf( "Try again.\n" );
	}
}

void ask_room_name( char* buf )
{
	printf( "How should I name your room?\n" );
	scanf( "%" STR_MAX_NAME_LEN "s", buf );
	/* check if valid and not duplicates */
}

void ask_player_name( char* buf )
{
	printf( "How should I name you?\n" );
	scanf( "%" STR_MAX_NAME_LEN "s", buf );
	/* TODO: check if valid and not duplicates */
}

int ask_host_action()
{
	int response;

	while( 1 ) {
		scanf( "%d", &response );
		switch( response ) {
			case 1:
				return A_START_GAME;
			case 2:
				return A_STOP_GAME;
			case 3:
				return A_CLOSE_ROOM;
			case 4:
				return A_ASK_PLAYER_LIST;
			case 5:
				return A_EXIT;
			default:
				break;
		}
		printf( "Try again.\n" );
	}
}

int ask_which_room( int room_count )
{
	int response;

	while( 1 ) {
		printf( "Which room do you choose?\n" );
		scanf( "%d", &response );
		if( response >= 0 && response < room_count ) {
			return response;
		}
		printf( "Try again.\n" );
	}
}


int getrecvlist( )
{
	int n, i;
	char** recvlist;
	CN1(blocking_read_int( poll_arr[P_SOCK].fd, &n ), E_LOSTCONNECTION); /* number of player names */
	printf( "We have %d items:\n", n );
	recvlist = malloc_s( n * sizeof( char* ));
	for( i = 0; i < n; ++i ) {
		recvlist[i] = calloc_s( MAX_NAME_LEN, sizeof(char) );
		blocking_read_buf( poll_arr[P_SOCK].fd, recvlist[i] );
	}

	for( i = 0; i < n; ++i ) {
		printf( "%d: %s\n", i, recvlist[i] );
		free( recvlist[i] );
	}
	free( recvlist );
	return n;
}

/** Get and print player's list */
int getplayerlist()
{
	int n, i;
	CN1(blocking_read_int( poll_arr[P_SOCK].fd, &n ), E_LOSTCONNECTION); /* number of player names */
	printf( "We have %d items:\n", n );

	for( i = 0; i < n; ++i ) {
		blocking_read_buf( poll_arr[P_SOCK].fd, (char*) &player );
		printf( "%d: %3d hp %2d mines %2d x %2d y %s \n", i, player.hp, player.num_of_mines, player.x, player.y, player.name );
	}

	return n;
}

/** Get screen and player info and render it */
void render()
{
	blocking_read_buf( poll_arr[P_SOCK].fd, (char*) &player );
	blocking_read_buf( poll_arr[P_SOCK].fd, buf );
	clear();
	printf( "%s", buf );
	printf( "%3d hp %2d mines %2dx %2dy %s\n", player.hp, player.num_of_mines, player.x, player.y, player.name );
}
