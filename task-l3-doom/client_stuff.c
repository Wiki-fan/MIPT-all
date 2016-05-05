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

char buf[CLIENT_BUF_SIZE];
int sockfd;
Player player;

int setup_connection()
{
	int sock_id, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent* server;

	char buffer[256];

	portno = PORT;

	CHN1( sock_id = socket( AF_INET, SOCK_STREAM, 0 ), 20, "Can't create socket" );

	CH0( server = gethostbyname( HOSTNAME ), 25, "No such hostname" );

	bzero((char*) &serv_addr, sizeof( serv_addr ));
	serv_addr.sin_family = AF_INET;
	/* TODO: macros expansion in macros. h_addr don't work */
	bcopy((char*) server->h_addr_list[0], (char*) &serv_addr.sin_addr.s_addr, server->h_length );
	serv_addr.sin_port = htons( portno );

	/* Now connect to the server */
	CHN1( connect( sock_id, (struct sockaddr*) &serv_addr, sizeof( serv_addr )), 26, "Can't connect to server" );

	return sock_id;
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
				return A_JOINROOM;
			case 3:
				return A_EXIT;
			default:
				break;
		}
		printf( "Try again.\n" );
	}
}

int ask_room_name( char* buf )
{
	printf( "How should I name your room?\n" );
	scanf( "%" STR_MAX_NAME_LEN "s", buf );
	/* check if valid and not duplicates */
}

int ask_player_name( char* buf )
{
	printf( "How should I name you?\n" );
	scanf( "%" STR_MAX_NAME_LEN "s", buf );
	/* check if valid and not duplicates */
}

int ask_host_action()
{
	int response;

	while( 1 ) {
		printf( "What do you want?\n1. Start game.\n2. Stop game.\n3. Get list of players in room.\n4. Exit.\n" );
		scanf( "%d", &response );
		switch( response ) {
			case 1:
				return A_START_GAME;
			case 2:
				return A_STOP_GAME;
			case 3:
				return A_ASK_PLAYER_LIST;
			case 4:
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
	n = read_int( sockfd ); /* number of player names */
	printf( "We have %d items:\n", n );
	recvlist = malloc_s( n * sizeof( char* ));
	for( i = 0; i < n; ++i ) {
		recvlist[i] = malloc_s( MAX_NAME_LEN );
		read_buf( sockfd, recvlist[i] );
	}

	for( i = 0; i < n; ++i ) {
		printf( "%d: %s\n", i, recvlist[i] );
		free( recvlist[i] );
	}
	free( recvlist );
	return n;
}

int getplayerlist()
{
	int n, i;
	n = read_int( sockfd ); /* number of player names */
	printf( "We have %d items:\n", n );

	for( i = 0; i < n; ++i ) {
		read_buf( sockfd, (char*) &player );
		printf( "%d: %3d hp %2d mines %2d x %2d y %s \n", i, player.hp, player.num_of_mines, player.x, player.y, player.name );
	}

	return n;
}

/* get screen and player info from sockfd and render it */
void render()
{
	read_buf( sockfd, (char*) &player );
	read_buf( sockfd, buf );
	clear();
	printf( "%s", buf );
	printf( "%3d hp %2d mines %2dx %2dy %s\n", player.hp, player.num_of_mines, player.x, player.y, player.name );
}
