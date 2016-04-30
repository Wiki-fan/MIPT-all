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
#include "data_stuff.h"

Vector_Room rooms;
Game game;

static pthread_mutex_t mtx_endqueue = PTHREAD_MUTEX_INITIALIZER;
fd_set master; /* master file descriptor list*/
fd_set read_fds; /* temp file descriptor list for select()*/
int fdmax; /* max num of file descriptor, so we check only [0, fdmax] */
int listener_fd, port_num = PORT;

GameQueue gq;
Vector_SockIdInfo sock_info;

int read_buf( int sock_id, char* buf )
{
	int n, count;
	CHN1( n = read( sock_id, &count, sizeof( int )), 22, "Can't read the socket" );
	CHN1( n = read( sock_id, buf, count ), 22, "Can't read the socket" );
	return count;
}

void send_buf( int sockfd, int count, char* buf )
{
	int n;
	CHN1( n = write( sockfd, &count, sizeof( int )), 27, "Socket write error" );
	CHN1( n = write( sockfd, buf, count ), 27, "Socket write error" );
}

int send_int( int act, int sockfd )
{
	int n;

	CHN1( n = write( sockfd, &act, sizeof( int )), 27, "Socket write error" );
	return 0;
}

int read_int( int sock_id )
{
	int state, n;
	CHN1( n = read( sock_id, &state, sizeof( int )), 22, "Can't read the socket" );
	return state;
}

void* server_loop( void* args )
{
	int i, j, n;
	int newsock_id;
	struct sockaddr_in cli_addr;
	socklen_t clilen;
	char buf[MAX_NAME_LEN];
	Room room;
	Player player;
	SockIdInfo info;
	int temp;

	while( 1 ) {
		read_fds = master;
		CHN1( select( fdmax + 1, &read_fds, NULL, NULL, NULL ), 26, "Error in select" );

		for( i = 0; i <= fdmax; i++ ) {
			if( FD_ISSET( i, &read_fds )) {
				/* accept new connection */
				if( i == listener_fd ) {

					clilen = sizeof( cli_addr );
					CHN1( newsock_id = accept( listener_fd, (struct sockaddr*) &cli_addr, &clilen ), 23, "Can't accept" );

					FD_SET( newsock_id, &master ); /* add to master set*/
					if( newsock_id > fdmax ) {    /* keep track of the max*/
						fdmax = newsock_id;
					}

					printf( "new connection: socket %d\n", i );
				} else {
					/* handle data from a client*/
					enum ACTION act;
					int nbytes;
					CHN1( nbytes = recv( i, &act, sizeof( enum ACTION ), 0 ), 27, "Error recv" );

					if( nbytes == 0 ) {
						/* connection closed*/
						/*printf( "Socket %d hung up, player %d from room %d disconnected\n", i, sock_info.arr[i].player_id, sock_info.arr[i].room_id );*/
						printf( "Socket %d hung up\n", i );
						close( i );
						FD_CLR( i, &master ); /* remove from master set*/
					} else {
						switch( act ) {
							case A_CREATEROOM:
								/*receive room name */
								n = read_buf( i, buf );
								/* init room struct */
								strcpy( room.name, buf );
								Vector_Player_init( &room.players, INITIAL_NUM_OF_PLAYERS );
								copy_map(&map, &room.map);

								/* writing that this socket is host */
								info.room_id = rooms.size;
								info.player_id = -1;
								Vector_SockIdInfo_set( &sock_info, info, i );

								/* add room */
								Vector_Room_push( &rooms, room );

								send_int( R_CREATED, i );
								LOG(( "Room with name %s was created by socket %d\n", room.name, i ));
								break;
							case A_ASK_PLAYER_LIST:
								send_int( R_SENDING_PLAYERS, i );
								temp = sock_info.arr[i].room_id; /* room id */
								send_int( rooms.arr[temp].players.size, i );

								for( j = 0; j < rooms.arr[temp].players.size; ++j ) {
									send_buf( i, (int)strlen( rooms.arr[temp].players.arr[j].name ), rooms.arr[temp].players.arr[j].name );
								}
								LOG(( "Send list of %d players of room %d to socket %d\n", rooms.arr[temp].players.size, temp, i ));
								break;
							case A_ASK_ROOMS_LIST:
								send_int( R_SENDING_ROOMS, i );
								send_int( rooms.size, i);

								for( j = 0; j < rooms.size; ++j ) {
									send_buf( i, (int)strlen( rooms.arr[j].name ), rooms.arr[j].name );
								}
								LOG(( "Send list of rooms to socket %d\n", i ));
								break;
							case A_JOINROOM:
								j = read_int( i ); /* room id */
								read_buf(i, buf); /* player name */
								player_init( &player );
								strcpy(player.name, buf);
								info.room_id = j;
								info.player_id = rooms.arr[info.room_id].players.size;
								Vector_Player_push( &rooms.arr[info.room_id].players, player );

								Vector_SockIdInfo_set( &sock_info, info, i );
								send_int(R_JOINED, i);
								LOG(( "Player from socket %d was added to room %d as player #%d\n", i, j, info.player_id ));
								break;
							case A_UP:
							case A_DOWN:
							case A_LEFT:
							case A_RIGHT:
							case A_MINE:
							case A_USE:
							case A_ATTACK:
								printf( "Player with id %d from room %d did action %d\n", sock_info.arr[i].player_id, sock_info.arr[i].room_id, act );
								CHN0( pthread_mutex_lock( &mtx_endqueue ), 30, "Error locking mutex" );
								GameQueue_push( &gq, sock_info.arr[i], act );
								CHN0( pthread_mutex_unlock( &mtx_endqueue ), 31, "Error unlocking mutex" );
								break;
							default:
								LOG(("Unknown command %d\n", act));

						}
					}
				}
			}
		}
	}
}

void* game_loop( void* args )
{
	Node* node;
	int x, y;
	Player* player;
	char c;
	String str;

	String_init(&str);
	GameQueue_init( &gq );
	while( 1 ) {
		node = NULL;
		CHN0( pthread_mutex_lock( &mtx_endqueue ), 30, "Error locking mutex" );
		if( !GameQueue_empty( &gq )) {
			node = GameQueue_pop( &gq );
		}
		CHN0( pthread_mutex_unlock( &mtx_endqueue ), 31, "Error unlocking mutex" );

		if( node != NULL) {
			LOG(( "Begin to process player %d from room %d with action %d\n", node->sock_info.player_id, node->sock_info.room_id, node->act ));
			switch( node->act ) {
				case A_UP:
					player_move( node->sock_info.room_id, node->sock_info.player_id, 0, -1 );
					break;
				case A_DOWN:
					player_move( node->sock_info.room_id, node->sock_info.player_id, 0, 1 );
					break;
				case A_LEFT:
					player_move( node->sock_info.room_id, node->sock_info.player_id, -1, 0 );
					break;
				case A_RIGHT:
					player_move( node->sock_info.room_id, node->sock_info.player_id, 1, 0 );
					break;
				case A_ATTACK:
					player_attack( node->sock_info.room_id, node->sock_info.player_id );
					break;
				case A_MINE:
					player_mine( node->sock_info.room_id, node->sock_info.player_id );
					break;
				case A_USE:
					player_use( node->sock_info.room_id, node->sock_info.player_id );
				default:
					break;
			}
			player = &rooms.arr[node->sock_info.room_id].players.arr[node->sock_info.player_id];
			for( y = player->y - FIELD_OF_SIGHT + 1; y < player->y + FIELD_OF_SIGHT; ++y ) {
				for( x = player->x - FIELD_OF_SIGHT + 1; x < player->x + FIELD_OF_SIGHT; ++x ) {
					if( x >= 0 && x < map.w && y >= 0 && y < map.h ) {
						if( player->x == x && player->y == y ) {
							String_push( &str, PLAYER );
						} else {
							String_push( &str, map.m[y][x] );
						}
					} else {
						String_push( &str, ' ' );
					}
				}
				String_push(&str, '\n' );
			}
			String_push( &str, '\0' );
			str.cur_pos = 0;
			printf("%s", str.buf);
		}
	}
}

void* response_loop(void* args)
{

}

/** Init server (open socket, bind, listen, spawn thread) */
int server_init()
{
	struct sockaddr_in serv_addr;
	pthread_t listener_pid, game_loop_pid, response_pid;

	LOG(( "Server started\n" ));

	CHN1( listener_fd = socket( AF_INET, SOCK_STREAM, 0 ), 20, "Can't create socket" );

	/* initialize socket structure */
	bzero((char*) &serv_addr, sizeof( serv_addr ));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons((uint16_t) port_num );

	/* bind the socket*/
	CHN1( bind( listener_fd, (struct sockaddr*) &serv_addr, sizeof( serv_addr )), 21, "Can't bind socket" );

	/* listen socket */
	CHN1( listen( listener_fd, BACKLOG ), 29, "Can't listen" );

	/* initialize fd_set structure */
	FD_ZERO( &master );
	FD_SET( listener_fd, &master );
	fdmax = listener_fd;

	/* game stuff init */
	Vector_Room_init( &rooms, INITIAL_NUM_OF_ROOMS );

	pthread_create( &listener_pid, NULL, server_loop, NULL);
	pthread_create( &game_loop_pid, NULL, game_loop, NULL);
	/*pthread_create( &response_pid, NULL, response_loop, NULL);*/
	/* this threads should normally last forever */
	pthread_join( listener_pid, NULL);
	pthread_join( game_loop_pid, NULL);

	/*bzero( buf,
		   256 );
	CHN1( n = read( newsock_id, buf, 255 ), 22, "Can't read the socket" );*/

	/*printf( "message: %s\n", buf );*/
/*printf("player) with id %d did action %d", buf[0], buf[4]);*/

/* Write a response to the client */
/*CHN1( n = write( newsock_id, "got message", 18 ), 23, "Can't write to the socket");*/
	return 0;
}

