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
#define CLIENT_BUF_SIZE 800
#define STR_CLIENT_BUF_SIZE "800"

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
		printf( "What do you want?\n1. Start game.\n2. Stop game.\n3. Get list of players in room.\n3. Exit.\n" );
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


int getrecvlist( int sockfd )
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

int getplayerlist( int sockfd )
{
	int n, i;
	Player player;
	n = read_int( sockfd ); /* number of player names */
	printf( "We have %d items:\n", n );

	for( i = 0; i < n; ++i ) {
		read_buf( sockfd, (char*) &player );
		printf( "%d: %3d hp %2d mines %2d x %2d y %s \n", i, player.hp, player.num_of_mines, player.x, player.y, player.name );
	}

	return n;
}


#define CHK_RESPONSE( NEEDED, MSG )\
if (read_int(sockfd) == NEEDED) {\
printf(MSG); putchar('\n');\
} else {\
errx(13, "Wrong response");\
}

char buf[CLIENT_BUF_SIZE];

/* get screen and player info from sockfd and render it */
void render( int sockfd, Player* player )
{
	read_buf( sockfd, (char*) player );
	read_buf( sockfd, buf );
	clear();
	printf( "%s", buf );
	printf( "%3d hp %2d mines %2dx %2dy %s\n", player->hp, player->num_of_mines, player->x, player->y, player->name );
}

int play( int sockfd )
{
	Player player;
	int i;
	int isexit;

	/* Receive rooms list  */
	send_int( A_ASK_ROOMS_LIST, sockfd );
	CHK_RESPONSE( R_SENDING_ROOMS, "List of rooms received" );
	printf( "Rooms:\n" );
	i = ask_which_room( getrecvlist( sockfd ));

	ask_player_name( buf );
	send_int( A_JOINROOM, sockfd );
	send_int( i, sockfd );
	send_buf( sockfd, MAX_NAME_LEN, buf );
	CHK_RESPONSE( R_JOINED, "You joined" );

	printf( "Waiting for game to start\n" );
	CHK_RESPONSE( R_GAME_STARTED, "Game started" );
	CHK_RESPONSE( R_DONE, "Game info sended" );

	set_canonical();
	render( sockfd, &player );
	while( 1 ) {
		enum ACTION act;
		int fl;
		fl = get_input( &act );

		if( fl == 0 ) {
			if( act == A_EXIT ) {
				isexit = 1;
				break;
			} else {
				/*printf( "Got input %d\n", act );*/
				send_int( act, sockfd );
				i = read_int( sockfd );
				switch( i ) {
					case R_ROOM_CLOSED:
						printf( "Host closed room\n" );
						isexit = 0;
						break;
					case R_GAME_STOPPED:
						printf( "Game finished\n" );
						isexit = 0;
						break;
					case R_DIED:
						printf( "YOU DIED!!!!\n\n" );
						isexit = 0;
						break;
					case R_DONE:
						render( sockfd, &player );
						break;
					default:
						errx( 13, "Wrong response" );
				}
				if( isexit == 0 ) {
					break;
				}
			}
		}
	}

	restore();
	return !isexit;
}

int main()
{
	int sockfd;
	int n;

	/* Determining role of client */
	int act = ask_player_or_host();

	/* Connect */
	sockfd = setup_connection();
	printf( "Connection established\n" );

	switch( act ) {
		case A_CREATE_ROOM:
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
						getplayerlist( sockfd );
						break;
					case A_EXIT:
						exit( 0 );
					default:
						errx( 3, "Unreachable code" );
				}
			}
			break;
		case A_JOINROOM:
			while( play( sockfd )) { }
			break;
		case A_EXIT:
			return 0;
		default:
			errx( 3, "Unreachable code" );
	}
	return 0;
}