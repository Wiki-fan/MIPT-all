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

define_vector(Player);
typedef struct {
	int room_id;
	Vector_Player players;
	Map map;
} Room;

define_vector(SockIdInfo);
define_vector(Room);
Vector_Room rooms;

Game game;

fd_set master;
/* master file descriptor list*/
fd_set read_fds;
/* temp file descriptor list for select()*/
int fdmax;
/* max num of file descriptor, so we check only [0, fdmax] */
int listener_fd, port_num = PORT;

void player_move(int room_id, int player_id, int x, int y)
{
	Player* player = &(rooms.arr[room_id].players.arr[player_id]);
	if (rooms.arr[room_id].map.m[player->y+y][player->x + x] != WALL) {
		player->x += x;
		player->y += y;
	}
}

void player_init(int room_id, int player_id)
{
	Player* player = &(rooms.arr[room_id].players.arr[player_id]);
	player->x = player->y = 1;
	player->num_of_mines = NUM_OF_MINES;
	player->hp = game.initial_health;
}

void player_mine(int room_id, int player_id)
{
	Player* player = &(rooms.arr[room_id].players.arr[player_id]);
	Map* map = &(rooms.arr[room_id].map);
	if (map->m[player->y][player->x] == SPACE) {
		map->m[player->y][player->x] = MINE;
		--player->num_of_mines;
	}
}

void player_use(int room_id, int player_id)
{
	Player* player = &(rooms.arr[room_id].players.arr[player_id]);
	Map* map = &(rooms.arr[room_id].map);
	if (map->m[player->y][player->x] == BONUS) {
		player->hp += map->b[player->y][player->x];
		--player->num_of_mines;
	}
}

void player_attack(int room_id, int player_id)
{
	/* TODO: labyrinth DFS */
}

GameQueue gq;
Vector_SockIdInfo sock_info;

void* server_loop( void* args )
{
	int i;
	int newsock_id;
	struct sockaddr_in cli_addr;
	socklen_t clilen;

	while( 1 ) {
		read_fds = master;
		CHN1( select( fdmax + 1, &read_fds, NULL, NULL, NULL ), 26, "Error in select" );

		for( i = 0; i <= fdmax; i++ ) {
			if( FD_ISSET( i, &read_fds )) {
				/* accept new connection */
				if( i == listener_fd ) {
					SockIdInfo info;
					Player player;
					clilen = sizeof( cli_addr );
					CHN1( newsock_id = accept( listener_fd, (struct sockaddr*) &cli_addr, &clilen ), 23, "Can't accept" );

					FD_SET( newsock_id, &master ); /* add to master set*/
					if( newsock_id > fdmax ) {    /* keep track of the max*/
						fdmax = newsock_id;
					}

					info.room_id = 0;
					info.player_id = rooms.arr[info.room_id].players.size;
					player.hp = 99;
					Vector_Player_push(&rooms.arr[info.room_id].players, player);
					Vector_SockIdInfo_set(&sock_info, info, newsock_id);
					printf( "new connection: room %d, player #%d\n", info.room_id, info.player_id );
				} else {
					/* handle data from a client*/
					enum ACTION act;
					int nbytes;
					CHN1( nbytes = recv( i, &act, sizeof( enum ACTION), 0 ), 27, "Error recv" );
					if( nbytes == 0 ) {
						/* connection closed*/
						printf( "select: socket %d hung up\n", i );
						close( i );
						FD_CLR( i, &master ); /* remove from master set*/
					} else {
						printf( "player with id %d from room %d did action %d\n", sock_info.arr[i].player_id, sock_info.arr[i].room_id, act );
						GameQueue_push(&gq, sock_info.arr[i], act);
					}
				}
			}
		}
	}
}

void* game_loop(void* args)
{
	GameQueue_init(&gq);
	Node* node;
	while(node = GameQueue_pop(&gq)) {
		switch(node->act) {
			case A_UP:
				player_move(node->sock_info.room_id, node->sock_info.player_id, 0, -1);
				break;
			case A_DOWN:
				player_move(node->sock_info.room_id, node->sock_info.player_id, 0, 1);
				break;
			case A_LEFT:
				player_move(node->sock_info.room_id, node->sock_info.player_id, -1, 0);
				break;
			case A_RIGHT:
				player_move(node->sock_info.room_id, node->sock_info.player_id, 1, 0);
				break;
			case A_ATTACK:
				player_attack(node->sock_info.room_id, node->sock_info.player_id);
				break;
			case A_MINE:
				player_mine(node->sock_info.room_id, node->sock_info.player_id);
				break;
			case A_USE:
				player_use(node->sock_info.room_id, node->sock_info.player_id);
			default:
				break;
		}
	}
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
	Vector_Room_init(&rooms, INITIAL_NUM_OF_ROOMS);
	/* TODO: temporary solution*/
	Room room;
	/*room.map = NULL;*/
	room.room_id = 0;
	Vector_Player_init(&room.players, INITIAL_NUM_OF_PLAYERS);
	Vector_Room_push(&rooms, room);


	pthread_create( &listener_pid, NULL, server_loop, NULL);
	pthread_join(listener_pid, NULL);

	/*bzero( buf,
		   256 );
	CHN1( n = read( newsock_id, buf, 255 ), 22, "Can't read the socket" );*/

	/*printf( "message: %s\n", buf );*/
/*printf("player) with id %d did action %d", buf[0], buf[4]);*/

/* Write a response to the client */
/*CHN1( n = write( newsock_id, "got message", 18 ), 23, "Can't write to the socket");*/

}

