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

enum ROLE {
	ROLE_HOST, ROLE_PLAYER
};

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
	/*printf( "message:" );
	bzero( buffer, 256 );
	fgets( buffer, 255, stdin );

	CHN1( n = write( sockfd, buffer, strlen( buffer )), 27, "Socket write error" );

	bzero( buffer, 256 );
	CHN1( n = read( sockfd, buffer, 255 ), 28, "Socket read error" );

	printf( "%s\n", buffer );
	return 0;*/

}

int ask_player_or_host()
{
	int response;

	while( 1 ) {
		printf( "What do you want?\n1. Create room on the server.\n2. Play in already created room.\n3. Exit.\n" );
		scanf( "%d", &response );
		if( response == 1 || response == 2 || response == 3 ) {
			return response;
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
		printf( "What do you want?\n1. Start game.\n2. Get list of players in room.\n3. Exit.\n" );
		scanf( "%d", &response );
		if( response == 1 || response == 2 || response == 3 ) {
			return response;
		}
		printf( "Try again.\n" );
	}
}

int ask_which_room(int room_count)
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


int getrecvlist(int sockfd)
{
	int n, i;
	char** recvlist;
	n = read_int( sockfd ); /* number of player names */
	printf("We have %d items:\n", n);
	recvlist = malloc_s( n*sizeof(char*));
	for (i = 0; i<n; ++i) {
		recvlist[i] = malloc_s( MAX_NAME_LEN);
		read_buf(sockfd, recvlist[i]);
	}

	for (i = 0; i<n; ++i) {
		printf("%d: %s\n", i, recvlist[i]);
		free(recvlist[i]);
	}
	free(recvlist);
	return n;
}

int getplayerlist(int sockfd)
{
	int n, i;
	Player player;
	n = read_int( sockfd ); /* number of player names */
	printf("We have %d items:\n", n);

	for (i = 0; i<n; ++i) {
		read_buf(sockfd, (char*)&player);
		printf("%d: %3d hp %2d mines %2d x %2d y %s \n", i, player.hp, player.num_of_mines, player.x, player.y, player.name);
	}

	return n;
}


#define CHK_RESPONSE(NEEDED, MSG) \
if (read_int(sockfd) == NEEDED) {\
printf(MSG);\
} else {\
errx(13, "Wrong response");\
}

int main()
{
	int sockfd;
	enum ROLE role;
	enum ACTION init_action;
	int n, i;
	char buf[CLIENT_BUF_SIZE];

	/* Determining role of client */
	switch( ask_player_or_host()) {
		case 3:
			exit( 0 );
		case 1:
			role = ROLE_HOST;
			break;
		case 2:
			role = ROLE_PLAYER;
			break;
		default:
			errx( 3, "Unreachable code" );
	}

	/* Connect */
	sockfd = setup_connection();
	printf("Connection established\n");

	if (role == ROLE_HOST) {
		/* Create room */
		ask_room_name( buf );
		send_int( A_CREATEROOM, sockfd );
		send_buf( sockfd, MAX_NAME_LEN, buf );
		CHK_RESPONSE( R_CREATED, "Room is created on the server\n" );

		/* Possible actions of host */
		while (1) {
			switch(ask_host_action()) {
				case 1:
					send_int( A_STARTGAME, sockfd );
					break;
				case 2:
					send_int( A_ASK_PLAYER_LIST, sockfd );
					CHK_RESPONSE( R_SENDING_PLAYERS, "Receiving players list\n" );
					printf("Players:\n");
					getplayerlist(sockfd);
					break;
				case 3:
					exit(0);
				default:
					errx( 3, "Unreachable code" );
			}
		}
	} else {
		/* Receive rooms list  */
		send_int( A_ASK_ROOMS_LIST, sockfd );
		CHK_RESPONSE(R_SENDING_ROOMS, "List of rooms received\n");
		printf("Rooms:\n");
		i = ask_which_room(getrecvlist(sockfd));

		ask_player_name( buf );
		send_int( A_JOINROOM, sockfd );
		send_int( i, sockfd );
		send_buf( sockfd, MAX_NAME_LEN, buf );
		CHK_RESPONSE( R_JOINED, "You joined\n" );

		set_canonical();
		while( 1 ) {
			enum ACTION act;
			int fl;
			fl = get_input( &act );

			if( fl == 0 ) {
				if( act == A_EXIT ) {
					break;
				} else {
					/*printf( "Got input %d\n", act );*/
					send_int( act, sockfd );
					read_buf(sockfd, buf);
					clear();
					printf("%s", buf);
				}
			}
		}

		restore();
	}
	return 0;
}