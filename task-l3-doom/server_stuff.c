#define _POSIX_C_SOURCE 200901L
#include "server_stuff.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <err.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include "../common/utils.h"
#include "game_stuff.h"
#include "tty_stuff.h"
#include "config_stuff.h"
#include "common_types.h"
#include "data_stuff.h"
#include "net_stuff.h"
#include "server_action_stuff.h"

pthread_mutex_t mtx_endqueue;
pthread_cond_t gq_cond;
/* master file descriptor list*/
fd_set master;
/* temp file descriptor list for select()*/
fd_set read_fds;
/* max num of file descriptor, so we check only [0, fdmax] */
int fdmax;

int listener_fd, port_num;
timer_t timerid;

pthread_t listener_pid, game_loop_pid;
int server_work = 1;
int exitcode = 0;

Vector_Room rooms;
Game game;
GameQueue gq;
Vector_SockIdInfo sock_info;

/** Loop where network input is processed. */
void* server_loop( void* args )
{
	int i;

	block_timer_signal();

	while( server_work ) {
		read_fds = master;
		CN1( select( fdmax + 1, &read_fds, NULL, NULL, NULL ), E_SELECT );

		for( i = 0; i <= fdmax; i++ ) {
			if( FD_ISSET( i, &read_fds )) {
				/* accept new connection */
				if( i == listener_fd ) {
					accept_connection(i);
				} else {
					/* handle data from a client*/
					int act;
					int resp;
					if( sock_info.arr[i].pending_action == A_NULL ) {
						resp = read_int( i, &act );
					} else {
						resp = 1;
						act = sock_info.arr[i].pending_action;
					}
					if( resp == 0 ) {
						continue; /* Read the rest later */
					} else if( resp == -1 ) {
						close_connection(i);
					} else {
						switch( act ) {
							case A_CREATE_ROOM:
								create_room(i);
								break;
							case A_CLOSE_ROOM:
								close_room(i);
								break;
							case A_ASK_PLAYER_LIST:
								ask_players_list(i);
								break;
							case A_ASK_ROOMS_LIST:
								send_rooms_list(i);
								break;
							case A_JOIN_ROOM:
								join_room(i);
								break;
							case A_START_GAME:
								start_game(i);
								break;
							case A_STOP_GAME:
								stop_game(i);
								break;
							case A_UP:
							case A_DOWN:
							case A_LEFT:
							case A_RIGHT:
							case A_MINE:
							case A_USE:
							case A_ATTACK:
							case A_NONE:
								do_action(i, act);
								break;
							default:
							LOG(( "Unknown command %d", act ));
						}
					}
				}
			}
		}
	}
	return 0;
}

String str;

/* Loop where game logic is processed. It takes data from game queue and performs actions */
/* It accesses: gq, players for each room: modify, rooms, sock_info: read */
void* game_loop( void* args )
{
	Node* node;
	int x, y;
	Player* player;
	Map* map;

	/* For macros variable capture */
	int player_id;

	block_timer_signal();

	String_init( &str );
	GameQueue_init( &gq );
	while( server_work ) {
		CHN0( pthread_mutex_lock( &mtx_endqueue ), 30, "Error locking mutex" );
		while( GameQueue_empty( &gq )) {
			pthread_cond_wait( &gq_cond, &mtx_endqueue );
		}
		node = GameQueue_pop( &gq );

		/*LOG(( "Begin to process player %d from room %d with action %d",
				node->sock_info.player_id, node->sock_info.room_id, node->act ));*/
		player = &rooms.arr[node->sock_info.room_id].players.arr[node->sock_info.player_id];
		map = &( rooms.arr[node->sock_info.room_id].map );
		/* TODO: we could create new room with same id. But we have only one room in the latest modification of task. */
		/* If player have not died in some ways and room exists */
		if (rooms.arr[node->sock_info.room_id].is_exists != 0
				&& ALIVE( &( rooms.arr[node->sock_info.room_id].players.arr[node->sock_info.player_id] ))) {
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
					if ( rooms.arr[node->sock_info.room_id].moratory == 0) /* Can't attack during moratory */
						player_attack( node->sock_info.room_id, node->sock_info.player_id );
					break;
				case A_MINE:
					player_mine( node->sock_info.room_id, node->sock_info.player_id );
					break;
				case A_USE:
					player_use( node->sock_info.room_id, node->sock_info.player_id );
					break;
				case A_NONE:
					player->is_moving = 0;
					break;
				default:
					errx( 3, "Unreachable code" );
			}

			if( player->hp <= 0 ) {
				LOG(( "Kill player %d from room %d (socket %d)", \
                node->sock_info.player_id, node->sock_info.room_id, node->sock_info.sock_id ));
				send_int( R_DIED, node->sock_info.sock_id );
				--rooms.arr[node->sock_info.room_id].left_alive;
				player_kill( player, map );
			} else {
				player_id = node->sock_info.player_id;
				/* Make player's view buffer. */
				for( y = player->y - FIELD_OF_SIGHT; y < player->y + FIELD_OF_SIGHT; ++y )
					String_push( &str, M_HORIZONTAL );
				String_push( &str, '\n' );
				for( y = player->y - FIELD_OF_SIGHT + 1; y < player->y + FIELD_OF_SIGHT - 1; ++y ) {
					String_push( &str, M_VERTICAL );
					for( x = player->x - FIELD_OF_SIGHT + 1; x < player->x + FIELD_OF_SIGHT - 1; ++x ) {
						if( x >= 0 && x < map->w && y >= 0 && y < map->h ) {
							if( player->x == x && player->y == y ) {
								String_push( &str, M_YOU );
							} else if( ISPLAYER( y, x )) {
								String_push( &str, M_PLAYER );
							} else if( ISOURMINE( y, x )) {
								String_push( &str, M_MINE );
							} else {
								String_push( &str, map->fg[y][x] );
							}
						} else {
							String_push( &str, M_WALL );
						}
					}
					String_push( &str, M_VERTICAL );
					String_push( &str, '\n' );
				}
				for( y = player->y - FIELD_OF_SIGHT; y < player->y + FIELD_OF_SIGHT; ++y )
					String_push( &str, M_HORIZONTAL );
				String_push( &str, '\n' );
				String_push( &str, '\0' );

				send_int( R_DONE, node->sock_info.sock_id );
				send_buf( node->sock_info.sock_id, sizeof( Player ), (char*) player );
				send_buf( node->sock_info.sock_id, str.cur_pos, str.buf );
				/*LOG(( "Info sent to socket %d", node->sock_info.sock_id ));*/

				/*printf("%s", str.buf);*/

				str.cur_pos = 0;
			}
		}

		CHN0( pthread_mutex_unlock( &mtx_endqueue ), 31, "Error unlocking mutex" );
	}
	return 0;
}

void server_cleanup()
{
	int i;
	stop_timer( timerid );
	LOG(( "Starting cleanup" ));
	GameQueue_destroy( &gq );
	LOG(( "GQ destroyed" ));
	for( i = 0; i < rooms.size; ++i ) {
		room_delete( &rooms.arr[i] );
	}
	LOG(( "Rooms destroyed" ));
	Vector_Room_destroy( &rooms );
	Vector_SockIdInfo_destroy( &sock_info );
	LOG(( "Vectors destroyed" ));
	map_delete( &game.map );
	LOG(( "Map destroyed" ));
	server_work = 0;
	free( str.buf );
	timer_delete( timerid );
	pthread_kill( listener_pid, SIGKILL );
	pthread_kill( game_loop_pid, SIGKILL );
}

static void handler( int sig )
{
	/*printf("Enter handler\n");*/
	switch( sig ) {
		case SIGTERM:
		LOG(( "Exiting on SIGTERM" ));
			server_cleanup();
			exitcode = SIGTERM;
			break;
		case SIGINT:
		LOG(( "Exiting on SIGINT" ));
			server_cleanup();
			exitcode = SIGINT;
			break;
		case SIGALRM:
			decrease_hp();
			break;
		case SIGUSR1:
			break;
		default:
			err( 3, "Unreachable code" );
	}
}

/** Init server (open socket, bind, listen, spawn threads) */
int server_start()
{
	struct sockaddr_in serv_addr;
	struct sigaction sa;
	struct itimerspec its;

	LOG(( "Server started" ));

	port_num = port;

	CN1( listener_fd = socket( AF_INET, SOCK_STREAM, 0 ), E_SOCKET );

	/* initialize socket structure */
	memset( &serv_addr, 0, sizeof( serv_addr ));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons((uint16_t) port_num );

	/* bind the socket*/
	CN1( bind( listener_fd, (struct sockaddr*) &serv_addr, sizeof( serv_addr )), E_BIND );

	/* listen socket */
	CN1( listen( listener_fd, BACKLOG ), E_LISTEN );

	/* initialize fd_set structure */
	FD_ZERO( &master );
	FD_SET( listener_fd, &master );
	fdmax = listener_fd;

	/* game stuff init */
	Vector_Room_init( &rooms, INITIAL_NUM_OF_ROOMS );

	block_timer_signal();
	pthread_mutex_init( &mtx_endqueue, 0 );
	pthread_cond_init( &gq_cond, 0 );
	pthread_create( &listener_pid, NULL, server_loop, NULL);
	pthread_create( &game_loop_pid, NULL, game_loop, NULL);
	/* this threads should normally last forever */

	/* Init timer parameters */
	its.it_interval.tv_sec = 0;
	its.it_interval.tv_nsec = (__syscall_slong_t) ( game.step_standard_delay * NSEC_IN_SEC );
	its.it_value = its.it_interval;

	/* Setting signal handler */
	memset( &sa, 0, sizeof( sa ));
	sa.sa_handler = handler;
	sigfillset( &sa.sa_mask );
	sa.sa_flags = SA_RESTART;
	CN1( sigaction( SIGTERM, &sa, NULL ), E_SIGACTION );
	CN1( sigaction( SIGINT, &sa, NULL ), E_SIGACTION );
	CN1( sigaction( SIGALRM, &sa, NULL ), E_SIGACTION );

	/* Start timer */
	start_timer( timerid, &its );

	pthread_join( listener_pid, NULL);
	pthread_join( game_loop_pid, NULL);
	return exitcode;
}
