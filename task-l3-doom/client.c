#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <err.h>

#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <fcntl.h>

#include <strings.h>
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
	/*printf( "message:" );
	bzero( buffer, 256 );
	fgets( buffer, 255, stdin );

	CHN1( n = write( sockfd, buffer, strlen( buffer )), 27, "Socket write error" );

	bzero( buffer, 256 );
	CHN1( n = read( sockfd, buffer, 255 ), 28, "Socket read error" );

	printf( "%s\n", buffer );
	return 0;*/

}

int send_to_server(enum ACTION act, int sockfd) {
	int n;

	CHN1( n = write( sockfd, &act, sizeof(enum ACTION)), 27, "Socket write error" );
}

int main()
{
	int sockfd;
	set_canonical();
	sockfd = setup_connection();

	while(1) {
		enum ACTION act;
		int fl;
		fl = get_input(&act);

		if (fl == 0) {
			if (act == A_EXIT) {
				break;
			} else {
				printf("Got input %d\n", act);
				send_to_server(act, sockfd);
			}
		}
	}

	restore();
	return 0;
}