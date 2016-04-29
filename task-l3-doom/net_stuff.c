#include "net_stuff.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <err.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <netdb.h>
#include <unistd.h>
#include <strings.h>
#include <pthread.h>
#include <unitypes.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include "../common/utils.h"
#include "game_stuff.h"
#include "tty_stuff.h"
#include "config_stuff.h"
#include "common_types.h"

int setup_server()
{

	fd_set master;    /* master file descriptor list*/
	fd_set read_fds;  /* temp file descriptor list for select()*/
	int sock_id, newsock_id, port_num = PORT;
	socklen_t clilen;
	int fdmax;
	char buf[256];
	struct sockaddr_in serv_addr, cli_addr;
	int n;
	int listener_pid;

	LOG( ("Server started\n") );
	/*pthread_create(&listener_pid,)*/
	CHN1( sock_id = socket( AF_INET, SOCK_STREAM, 0 ), 20, "Can't create socket" );

	/* initialize socket structure */
	bzero((char*) &serv_addr, sizeof( serv_addr ));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons((uint16_t) port_num );

	/* bind the socket*/
	CHN1( bind( sock_id, (struct sockaddr*) &serv_addr, sizeof( serv_addr )), 21, "Can't bind socket" );

	/* listen socket */
	CHN1(listen( sock_id, BACKLOG ), 29, "Can't listen");

	/* initialize fd_set structure */
	FD_ZERO(&master);
	FD_SET( sock_id, &master );
	fdmax = sock_id;

	while( 1 ) {
		int i;
		read_fds = master;
		CHN1( select( fdmax + 1, &read_fds, NULL, NULL, NULL ), 26, "Error in select" );

		for( i = 0; i <= fdmax; i++ ) {
			if( FD_ISSET( i, &read_fds )) {
				/* accept new connection */
				if( i == sock_id ) {
					clilen = sizeof( cli_addr );
					CHN1( newsock_id = accept( sock_id, (struct sockaddr*) &cli_addr, &clilen ), 23, "Can't accept" );

					FD_SET( newsock_id, &master ); /* add to master set*/
					if( newsock_id > fdmax ) {    /* keep track of the max*/
						fdmax = newsock_id;
					}
					printf( "new connection\n" );
				} else {
					/* handle data from a client*/
					Action action;
					int nbytes;
					bzero( buf, 256 );
					CHN1( nbytes = recv( i, &action, sizeof( Action ), 0 ), 27, "Error recv" );
					if( nbytes == 0 ) {
						/* connection closed*/
						printf( "select: socket %d hung up\n", i );
						close( i );
						FD_CLR( i, &master ); /* remove from master set*/
					} else {
						printf("player with id %d did action %d\n", action.player_id, action.action);
					}
				}
			}
		}


/*CHN1( fcntl( newsock_id, F_SETFL, O_NONBLOCK ), 24, "Can't set nonblock" );*/

		/*bzero( buf,
		       256 );
		CHN1( n = read( newsock_id, buf, 255 ), 22, "Can't read the socket" );*/

		/*printf( "message: %s\n", buf );*/
/*printf("player) with id %d did action %d", buf[0], buf[4]);*/

/* Write a response to the client */
/*CHN1( n = write( newsock_id, "got message", 18 ), 23, "Can't write to the socket");*/
	}
}