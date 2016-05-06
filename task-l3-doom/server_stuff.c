#include "server_stuff.h"
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
#include "net_stuff.h"

static pthread_mutex_t mtx_endqueue;
static pthread_cond_t gq_cond;
fd_set master; /* master file descriptor list*/
fd_set read_fds; /* temp file descriptor list for select()*/
int fdmax; /* max num of file descriptor, so we check only [0, fdmax] */

int listener_fd, port_num = PORT;

Vector_Room rooms;
Game game;

GameQueue gq;
Vector_SockIdInfo sock_info;

/** Send player that he is a cheater and kill him. */
void ban(int sock) {

	int room_id = sock_info.arr[sock].room_id;
	Player* player = &rooms.arr[room_id].players.arr[sock_info.arr[sock].player_id];
	LOG(("Player %d from room %d, socket %d, was banned",
			sock_info.arr[sock].player_id, sock_info.arr[sock].room_id, sock_info.arr[sock].sock_id));
	send_int( R_CHEATER, sock );
	player_kill(player, &rooms.arr[room_id].map);
}

#define ISHOST(sock) (sock_info.arr[sock].player_id == -1)
#define MARKHOST(info) info.player_id = -1;

/** Loop where network input is processed. */
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
	int room_id;

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
					if( newsock_id > fdmax ) { /* keep track of the max*/
						fdmax = newsock_id;
					}

					LOG(( "new connection: socket %d\n", newsock_id ));
				} else {
					/* handle data from a client*/
					enum ACTION act;
					int nbytes;
					CHN1( nbytes = recv( i, &act, sizeof( enum ACTION ), 0 ), 27, "Error recv" );

					if( nbytes == 0 ) {
						/* connection closed*/
						/*printf( "Socket %d hung up, player %d from room %d disconnected\n", i, sock_info.arr[i].player_id, sock_info.arr[i].room_id );*/
						LOG(( "Socket %d hung up", i ));
						close( i );
						FD_CLR( i, &master ); /* remove from master set*/
					} else {
						switch( act ) {
							case A_CREATE_ROOM:
								/*receive room name */
								n = read_buf( i, buf );

								/* init room struct */
								strcpy( room.name, buf );
								room.is_started = 0;
								room.is_exists = 1;
								Vector_Player_init( &room.players, INITIAL_NUM_OF_PLAYERS );
								map_copy( &game.map, &room.map );

								/* writing that this socket is host */
								info.room_id = rooms.size;
								MARKHOST(info);
								info.sock_id = i;
								Vector_SockIdInfo_set( &sock_info, info, i );

								/* add room */
								Vector_Room_add( &rooms, room );

								send_int( R_ROOM_CREATED, i );
								LOG(( "Room #%d with name %s was created by socket %d", info.room_id, room.name, i ));
								break;
							case A_CLOSE_ROOM:
								if (!ISHOST(i)) {
									/* Ban ordinary player who wants to close room as host */
									ban(i);
								} else {
									room_id = sock_info.arr[i].room_id;
									send_to_all_in_room( room_id, R_ROOM_CLOSED );
									Vector_Player_destroy( &rooms.arr[room_id].players );
									room_delete(&rooms.arr[i]);
									LOG(( "Room %d closed", room_id ));
								}
								break;
							case A_ASK_PLAYER_LIST:
								if (!ISHOST(i)) {
									/* Ban ordinary player who wants to know all about other players */
									ban(i);
								} else {
									room_id = sock_info.arr[i].room_id; /* room id */
									send_int( R_SENDING_PLAYERS, i );
									send_int( rooms.arr[room_id].players.size, i );

									for( j = 0; j < rooms.arr[room_id].players.size; ++j ) {
										send_buf( i, sizeof( Player ), (char*) &rooms.arr[room_id].players.arr[j] );
									}
									LOG(( "Send list of %d players of room %d to socket %d", rooms.arr[room_id].players.size, room_id, i ));
								}
								break;
							case A_ASK_ROOMS_LIST:
								send_int( R_SENDING_ROOMS, i );
								send_int( rooms.size, i );

								for( j = 0; j < rooms.size; ++j ) {
									send_buf( i, (int) strlen( rooms.arr[j].name ), rooms.arr[j].name );
								}
								LOG(( "Send list of rooms to socket %d", i ));
								break;
							case A_JOINROOM:
								room_id = read_int( i ); /* room id */
								read_buf( i, buf ); /* player name */
								player_init( &player, &rooms.arr[room_id].map, buf, i );

								info.room_id = room_id;
								info.player_id = rooms.arr[info.room_id].players.size;
								info.sock_id = i;
								Vector_Player_push( &rooms.arr[info.room_id].players, player );

								Vector_SockIdInfo_set( &sock_info, info, i );
								send_int( R_JOINED, i );
								LOG(( "Player from socket %d was added to room %d as player #%d", i, room_id, info.player_id ));

								break;
							case A_START_GAME:
								if (!ISHOST(i)) {
									/* Ban ordinary player who wants to start game */
									ban(i);
								} else {
									room_id = sock_info.arr[i].room_id;
									rooms.arr[room_id].is_started = 1;
									send_to_all_in_room( room_id, R_GAME_STARTED );
									send_int( R_GAME_STARTED, i );
									/*CHN0( pthread_mutex_lock( &mtx_endqueue ), 30, "Error locking mutex" );
									for( j = 0; j < rooms.arr[room_id].players.size; ++j ) {
										GameQueue_push( &gq, sock_info.arr[rooms.arr[room_id].players.arr[j].sock], A_NONE );
									}
									CHN0( pthread_mutex_unlock( &mtx_endqueue ), 31, "Error unlocking mutex" );*/
									LOG(( "Game in room %d started", sock_info.arr[i].room_id ));
								}
								break;
							case A_STOP_GAME:
								if (!ISHOST(i)) {
									/* Ban ordinary player who wants to stop game */
									ban(i);
								} else {
									send_to_all_in_room( sock_info.arr[i].room_id, R_GAME_STOPPED );
									LOG(( "Game in room %d stopped", sock_info.arr[i].room_id ));
								}
								break;
							case A_UP:
							case A_DOWN:
							case A_LEFT:
							case A_RIGHT:
							case A_MINE:
							case A_USE:
							case A_ATTACK:
							case A_NONE:
								LOG(( "Player with id %d from room %d did action %d (socket %d)",
										sock_info.arr[i].player_id, sock_info.arr[i].room_id, act, i ));
								if( rooms.arr[sock_info.arr[i].room_id].is_started == 0 ) {
									/* ban player who moves before the game is started */
									ban(i);
								} else {
									CHN0( pthread_mutex_lock( &mtx_endqueue ), 30, "Error locking mutex" );
									GameQueue_push( &gq, sock_info.arr[i], act );
									pthread_cond_signal(&gq_cond);
									CHN0( pthread_mutex_unlock( &mtx_endqueue ), 31, "Error unlocking mutex" );
								}
								break;
							default:
							LOG(( "Unknown command %d", act ));
						}
					}
				}
			}
		}
	}
}

/* Loop where game logic is processed. It takes data from game queue and performs actions */
/* It accesses: gq, players for eack room: modify, rooms, sock_info: read */
void* game_loop( void* args )
{
	Node* node;
	int x, y;
	Player* player;
	Map* map;
	String str;

	/* For macros variable capture */
	int player_id;

	String_init( &str );
	GameQueue_init( &gq );
	while( 1 ) {
		node = NULL;
		CHN0( pthread_mutex_lock( &mtx_endqueue ), 30, "Error locking mutex" );
		while(GameQueue_empty(&gq)) {
			pthread_cond_wait(&gq_cond, &mtx_endqueue);
		}
		node = GameQueue_pop( &gq );

		if( node != NULL) {
			LOG(( "Begin to process player %d from room %d with action %d", node->sock_info.player_id, node->sock_info.room_id, node->act ));
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
					break;
				case A_NONE:
					break;
				default:
					errx(3, "Unreachable code");
			}

			player = &rooms.arr[node->sock_info.room_id].players.arr[node->sock_info.player_id];
			map = &( rooms.arr[node->sock_info.room_id].map );
			if( player->hp <= 0 ) {
				LOG(("Kill player %d from room %d (socket %d)", \
				node->sock_info.player_id, node->sock_info.room_id, node->sock_info.sock_id));
				send_int( R_DIED, node->sock_info.sock_id );
				player_kill(player, map);
			} else {

				player_id = node->sock_info.player_id;
				/* Make player's view buffer. */
				for( y = player->y - FIELD_OF_SIGHT + 1; y < player->y + FIELD_OF_SIGHT-1; ++y ) {
					for( x = player->x - FIELD_OF_SIGHT + 1; x < player->x + FIELD_OF_SIGHT-1; ++x ) {
						if( x >= 0 && x < map->w && y >= 0 && y < map->h ) {
							if( player->x == x && player->y == y ) {
								String_push( &str, YOU );
							} else if( ISPLAYER( y, x )) {
								String_push( &str, PLAYER );
							} else if( ISOURMINE( y, x )) {
								String_push( &str, MINE );
							} else {
								String_push( &str, map->fg[y][x] );
							}
						} else {
							String_push( &str, ' ' );
						}
					}
					String_push( &str, '\n' );
				}
				String_push( &str, '\0' );

				send_int( R_DONE, node->sock_info.sock_id );
				send_buf( node->sock_info.sock_id, sizeof( Player ), (char*) player );
				send_buf( node->sock_info.sock_id, str.cur_pos, str.buf );
				LOG(("Info sent to socket %d",node->sock_info.sock_id ));

				/*printf("%s", str.buf);*/

				str.cur_pos = 0;
			}
		}
		CHN0( pthread_mutex_unlock( &mtx_endqueue ), 31, "Error unlocking mutex" );
	}
}

/** TODO: decide, if we need one. */
void* response_loop( void* args )
{

}

void server_cleanup()
{
	int i;
	GameQueue_destroy(&gq);
	for (i = 0; i<rooms.size; ++i) {
		room_delete(&rooms.arr[i]);
	}
	Vector_Room_destroy(&rooms);
	Vector_SockIdInfo_destroy(&sock_info);

	map_delete(&game.map);
}

/** Init server (open socket, bind, listen, spawn threads) */
int server_start()
{
	struct sockaddr_in serv_addr;
	pthread_t listener_pid, game_loop_pid, response_pid;

	LOG(( "Server started" ));

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

	pthread_mutex_init(&mtx_endqueue, 0);
	pthread_cond_init(&gq_cond, 0);
	pthread_create( &listener_pid, NULL, server_loop, NULL);
	pthread_create( &game_loop_pid, NULL, game_loop, NULL);
	/* pthread_create( &response_pid, NULL, response_loop, NULL); */
	/* this threads should normally last forever */
	pthread_join( listener_pid, NULL);
	pthread_join( game_loop_pid, NULL);

	return 0;
}

