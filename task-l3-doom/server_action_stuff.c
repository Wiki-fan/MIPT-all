#include "server_action_stuff.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <err.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include "game_stuff.h"
#include "net_stuff.h"
#include "common_types.h"

extern fd_set master;
extern fd_set read_fds;
extern int fdmax;
extern int listener_fd;
extern Vector_Room rooms;
extern Game game;
extern GameQueue gq;
extern Vector_SockIdInfo sock_info;
extern pthread_mutex_t mtx_endqueue;
extern pthread_cond_t gq_cond;

/** Send player that he is a cheater and kill him. */
void ban( int sock )
{
	int room_id = sock_info.arr[sock].room_id;
	Player* player = &rooms.arr[room_id].players.arr[sock_info.arr[sock].player_id];
	LOG(( "Player %d from room %d, socket %d, was banned",
			sock_info.arr[sock].player_id, sock_info.arr[sock].room_id, sock_info.arr[sock].sock_id ));
	send_int( R_CHEATER, sock );
	player_kill( player, &rooms.arr[room_id].map );
	player_close_socket( player );
}

void decrease_hp()
{
	/*LOG(("Decrease HP on timer"));*/
	int i, j;
	Player* player;
	for( i = 0; i < rooms.size; ++i ) {
		/* If room exists and game in room is started */
		if( rooms.arr[i].is_exists == 1 && rooms.arr[i].is_started == 1 ) {
			for( j = 0; j < rooms.arr[i].players.size; ++j ) {
				player = &rooms.arr[i].players.arr[j];
				if( ALIVE( player )) {
					if( rooms.arr[i].left_alive <= 1 ) {
						player_kill(player, &rooms.arr[sock_info.arr[player->sock].room_id].map);
						send_to_all_in_room( i, R_GAME_FINISHED );
						send_int( R_GAME_FINISHED, rooms.arr[i].host_sockid ); /* Inform host */
						rooms.arr[i].is_started = 0;
						continue;
					}
					player_damage( player, -(player->is_moving?game.movement_health_drop:game.stay_health_drop) );
					if( rooms.arr[i].frames ) {
						/* Decrease player things */
						if( player->cooldown > 0 ) {
							--player->cooldown;
						}
						if( player->mining > 0 ) {
							--player->mining;
						}
					}
				}
			}
			if( rooms.arr[i].frames > 0 ) {
				--rooms.arr[i].frames;
			} else {
				/* Decrease room things */
				if( rooms.arr[i].moratory > 0 ) {
					--rooms.arr[i].moratory;
				}
				rooms.arr[i].frames = game.fps - 1;
			}
		}
	}
}

void close_room( int i )
{
	int room_id;
	if( !ISHOST( i )) {
		/* Ban ordinary player who wants to close room as host */
		ban( i );
	} else {
		room_id = sock_info.arr[i].room_id;

		rooms.arr[room_id].is_started = 0;
		rooms.arr[room_id].is_exists = 0;
		send_to_all_in_room( room_id, R_ROOM_CLOSED );
		send_int( R_ROOM_CLOSED, i );
		room_delete( &rooms.arr[room_id] );
		LOG(( "Room %d closed", room_id ));
	}
}

void start_game( int i )
{
	int room_id, j;
	Player* player;
	if( !ISHOST( i )) {
		/* Ban ordinary player who wants to start game */
		ban( i );
	} else {
		room_id = sock_info.arr[i].room_id;
		rooms.arr[room_id].left_alive = rooms.arr[room_id].players.size;
		for( j = 0; j < rooms.arr[room_id].players.size; ++j ) {
			player = &rooms.arr[room_id].players.arr[j];
			/* Disconnected players should stay dead. They will present in room, but not in stat. */
			if( player->sock != -1 ) {
				/* Reset player's position and characteristics */
				player_init( &rooms.arr[room_id].players.arr[j], &rooms.arr[room_id].map );
				rooms.arr[room_id].map.pl[player->y][player->x] = j;
			}
		}
		send_to_all_in_room( room_id, R_GAME_STARTED );
		send_int( R_GAME_STARTED, i ); /* Inform host */
		LOG(( "Game in room %d started", sock_info.arr[i].room_id ));
		/* Setting flag AFTER all initialization */
		rooms.arr[room_id].is_started = 1;
	}
}

void send_rooms_list( int i )
{
	int j;
	int room_count = 0;
	send_int( R_SENDING_ROOMS, i );
	for( j = 0; j < rooms.size; ++j ) {
		if( rooms.arr[j].is_exists ) {
			++room_count;
		}
	}
	send_int( room_count, i );
	for( j = 0; j < rooms.size; ++j ) {
		if( rooms.arr[j].is_exists ) {
			send_int( j, i );
			send_buf( i, (int) strlen( rooms.arr[j].name ), rooms.arr[j].name );
		}
	}
	LOG(( "Send list of rooms to socket %d", i ));
}

void create_room( int i)
{
	SockIdInfo* info = &sock_info.arr[i];
	Room room;
	/*receive room name */
	read_buf( i, buf );

	/* init room struct */
	strcpy( room.name, buf );
	room.is_started = 0;
	room.is_exists = 1;
	room.host_sockid = i;
	room.frames = game.fps - 1;
	room.moratory = game.moratory_duration;
	Vector_Player_init( &room.players, INITIAL_NUM_OF_PLAYERS );
	map_copy( &game.map, &room.map );

	/* Mark that this socket is host */
	MARKHOST(( *info ));

	/* add room */
	info->room_id = rooms.size;
	Vector_Room_add( &rooms, room );

	send_int( R_ROOM_CREATED, i );
	LOG(( "Room #%d with name %s was created by socket %d", info->room_id, room.name, i ));
}

void join_room( int i )
{
	int room_id;
	Player player;
	SockIdInfo* info = &sock_info.arr[i];

	read_buf( i, buf ); /* player name */
	room_id = *(int*) buf;

	if( rooms.arr[room_id].is_started ) {
		send_int( R_ALREADY_STARTED, i );
	}
	/*player_init( &player, &rooms.arr[room_id].map );*/
	memset(&player, 0, sizeof(Player));
	player.sock = i;
	/* Name */
	memset( player.name, 0, MAX_NAME_LEN );
	strcpy( player.name, buf + sizeof( int ));

	/* SockInfo instance */
	info->room_id = room_id;
	info->player_id = rooms.arr[info->room_id].players.size;
	info->sock_id = i;
	/*rooms.arr[room_id].map.pl[player.y][player.x] = info->player_id;*/
	Vector_Player_push( &rooms.arr[info->room_id].players, player );

	send_int( R_JOINED, i );
	send_buf( i, sizeof( float ), (char*) &game.step_standard_delay );
	LOG(( "Player from socket %d was added to room %d as player #%d", i, room_id, info->player_id ));
}

void ask_players_list( int i )
{
	if( !ISHOST( i )) {
		/* Ban ordinary player who wants to know all about other players */
		ban( i );
	} else {
		int j;
		int room_id;
		int count = 0;
		room_id = sock_info.arr[i].room_id; /* room id */
		send_int( R_SENDING_PLAYERS, i );
		for( j = 0; j < rooms.arr[room_id].players.size; ++j ) {
			if( rooms.arr[room_id].players.arr[j].sock != -1 ) {
				++count;
			}
		}
		send_int( count, i );

		for( j = 0; j < rooms.arr[room_id].players.size; ++j ) {
			if( rooms.arr[room_id].players.arr[j].sock != -1 ) {
				send_buf( i, sizeof( Player ), (char*) &rooms.arr[room_id].players.arr[j] );
			}
		}
		LOG(( "Send list of %d players of room %d to socket %d", rooms.arr[room_id].players.size, room_id, i ));
	}
}

void stop_game( int i )
{
	if( !ISHOST( i )) {
		/* Ban ordinary player who wants to stop game */
		ban( i );
	} else {
		int room_id;
		SockIdInfo* info = &sock_info.arr[i];
		int j;
		room_id = sock_info.arr[i].room_id;

		rooms.arr[room_id].is_started = 0;
		send_to_all_in_room( sock_info.arr[i].room_id, R_GAME_STOPPED );
		send_int( R_GAME_STOPPED, i ); /* Inform host */
		for( j = 0; j < rooms.arr[room_id].players.size; ++j ) {
			Player* player = &rooms.arr[room_id].players.arr[j];
			/* Reset player's position and characteristics */
			rooms.arr[room_id].map.pl[player->y][player->x] = -1;
			player_init( player, &rooms.arr[room_id].map );
			rooms.arr[room_id].map.pl[player->y][player->x] = info->player_id;
		}
		LOG(( "Game in room %d stopped", sock_info.arr[i].room_id ));
	}
}

void do_action( int i, int act )
{
	/*LOG(( "Player with id %d from room %d did action %d (socket %d)",
										sock_info.arr[i].player_id, sock_info.arr[i].room_id, act, i ));*/
	if( rooms.arr[sock_info.arr[i].room_id].is_started == 0 ) {
		/* ban player who moves before the game is started */
		/*ban( i );*/
	} else {
		CHN0( pthread_mutex_lock( &mtx_endqueue ), 30, "Error locking mutex" );
		GameQueue_push( &gq, sock_info.arr[i], act );
		pthread_cond_signal( &gq_cond );
		CHN0( pthread_mutex_unlock( &mtx_endqueue ), 31, "Error unlocking mutex" );
	}
}

void close_connection( int i )
{
	/* connection closed*/
	if( ISHOST( i )) {
		close_room( i );
		close( i );
		FD_CLR( i, &master ); /* remove from master set*/
	} else {
		int room_id;
		Player* player;
		room_id = sock_info.arr[i].room_id;
		player = &rooms.arr[room_id].players.arr[sock_info.arr[i].player_id];
		--rooms.arr[room_id].left_alive;
		player_kill( player, &rooms.arr[room_id].map );
		player_close_socket( player );
	}
	LOG(( "Socket %d hung up", i ));
}

void accept_connection()
{
	struct sockaddr_in cli_addr;
	int newsock_id;
	socklen_t clilen;
	SockIdInfo* info;

	clilen = sizeof( cli_addr );
	CN1( newsock_id = accept( listener_fd, (struct sockaddr*) &cli_addr, &clilen ), E_ACCEPT );

	FD_SET( newsock_id, &master ); /* add to master set*/
	if( newsock_id > fdmax ) { /* keep track of the max*/
		fdmax = newsock_id;
	}

	Vector_SockIdInfo_alloc( &sock_info, newsock_id );
	info = &sock_info.arr[newsock_id];
	info->sock_id = newsock_id;
	info->reading_what = READING_NOTHING;
	info->pending_action = A_NULL;
	MARKUNINITIALIZED( info );

	LOG(( "new connection: socket %d\n", newsock_id ));
}
