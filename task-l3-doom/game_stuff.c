#include "game_stuff.h"
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

extern Vector_Room rooms;
extern Game game;
extern Vector_SockIdInfo sock_info;
extern fd_set master;

/** Add room to first free place in room vector. */
void Vector_Room_add(Vector_Room* rooms, Room elem)
{
	int i;
	for (i = 0; i<rooms->size; ++i) {
		if (rooms->arr[i].is_exists == 0) {
			rooms->arr[i] = elem;
			return;
		}
	}
	Vector_Room_push(rooms, elem);
}

void GameQueue_init( GameQueue* q )
{
	q->begin = q->end = NULL;
}

void GameQueue_push( GameQueue* q, SockIdInfo sock_info, enum ACTION act )
{
	Node* temp = malloc_s( sizeof( Node ));
	temp->sock_info = sock_info;
	temp->act = act;
	temp->next = NULL;
	if( q->end == NULL) {
		q->begin = q->end = temp;
	} else {
		q->end->next = temp;
	}
}

Node* GameQueue_pop( GameQueue* q )
{
	Node* temp = q->begin;
	q->begin = q->begin->next;
	if( q->begin == NULL) {
		q->end = NULL;
	}
	return temp;
}

void GameQueue_destroy( GameQueue* q )
{
	Node* temp;
	while( q->begin != NULL) {
		temp = q->begin;
		q->begin = q->begin->next;
		free( temp );
	}
}

int GameQueue_empty( GameQueue* q )
{
	return ( q->begin == NULL);
}

/** Fill room's map using base map. */
void map_copy( Map* from, Map* to )
{
	int i, j;
	to->h = from->h;
	to->w = from->w;
	to->fg = (char**) malloc_s( to->h * sizeof( char* ));
	to->bg = (int**) malloc_s( to->h * sizeof( int* ));
	to->pl = (int**) malloc_s( to->h * sizeof( int* ));
	for( i = 0; i < to->h; ++i ) {
		to->fg[i] = (char*) malloc_s(( to->w ) * sizeof( char ));
		to->bg[i] = (int*) malloc_s(( to->w ) * sizeof( int ));
		to->pl[i] = (int*) malloc_s(( to->w ) * sizeof( int ));
		memcpy( to->fg[i], from->fg[i], to->w * sizeof( char ));
		memcpy( to->bg[i], from->bg[i], to->w * sizeof( int ));
		for( j = 0; j < to->w; ++j ) {
			to->pl[i][j] = -1;
		}
	}
}

void map_delete(Map* map)
{
	int i;
	for (i = 0; i<map->h; ++i) {
		free(map->fg[i]);
		free(map->bg[i]);
		free(map->pl[i]);
	}
	free(map->fg);
	free(map->bg);
	free(map->pl);
}

/** Delete room, free map and player vector, mark room as nonexistent. */
void room_delete(Room* room)
{
	map_delete(&room->map);
	Vector_Player_destroy(&room->players);
	/*sock_info.arr[room.]*/ /* TODO: delete socket? */
	room->is_exists = 0;
}

/** Change player's health on value points. Returns 0, if player was killed. */
int player_damage( Player* player, int value )
{
	player->hp += value;
	return player->hp > 0;
}

/** Kill player and move it out of map. Close socket. Send him that he died before calling function. */
void player_kill(Player* player, Map* map)
{
	map->pl[player->y][player->x] = -1;
	player->x = -1;
	player->y = -1;
	close(sock_info.arr[player->sock].sock_id);
	FD_CLR( player->sock, &master ); /* remove from master set*/
	player->sock = -1;
}

/** Move player on (x, y). */
void player_move( int room_id, int player_id, int x, int y )
{
	Player* player = &( rooms.arr[room_id].players.arr[player_id] );
	Map* map = &rooms.arr[room_id].map;
	/* if we go to bonus or space, just move */
	if( ISSPACE( player->y + y, player->x + x ) || ISBONUS( player->y + y, player->x + x ) || ISOURMINE( player->y + y, player->x + x )) {
		if( !ISOURMINE( player->y, player->x )) {
			map->pl[player->y][player->x] = -1;
		}
		player->x += x;
		player->y += y;
		map->pl[player->y][player->x] = player_id;
	} else /* if we stepped on mine, and mine is not our mine */
	if( ISMINE( player->y + y, player->x + x )) {
		map->pl[player->y][player->x] = -1;
		player->x += x;
		player->y += y;
		player_damage(player, map->bg[player->y][player->x]);
		map->bg[player->y][player->x] = 0;
		map->pl[player->y][player->x] = player_id;
	}
}

/** Initial player's parameters. */
void player_init( Player* player, Map* map, char* name, int sock )
{
	/* Place player randomly */
	int fl = 1;
	while( fl ) {
		player->x = rand() % ( game.map.w - 2 ) + 1;
		player->y = rand() % ( game.map.h - 2 ) + 1;
		if( /*game.map.fg[player->y][player->x] == SPACE */ISSPACE(player->y, player->x)) {
			fl = 0;
		}
	}

	player->num_of_mines = NUM_OF_MINES;
	player->hp = game.initial_health;
	player->sock = sock;
	memset(player->name, 0, MAX_NAME_LEN);
	strcpy( player->name, name );
}

/** Set mine on this tile */
void player_mine( int room_id, int player_id )
{
	Player* player = &( rooms.arr[room_id].players.arr[player_id] );
	Map* map = &( rooms.arr[room_id].map );
	if( player->num_of_mines > 0 && map->fg[player->y][player->x] == SPACE
			&& !ISOURMINE( player->y, player->x )) { /* We can't place mine on bonus or other mine */
		/*map->fg[player->y][player->x] = MINE;*/
		map->pl[player->y][player->x] = player_id;
		map->bg[player->y][player->x] = -game.hit_value;
		--player->num_of_mines;
	}
}

/** Use bonus on this tile */
void player_use( int room_id, int player_id )
{
	Player* player = &( rooms.arr[room_id].players.arr[player_id] );
	Map* map = &( rooms.arr[room_id].map );
	if( ISBONUS( player->y, player->x )) {
		player_damage( player, map->bg[player->y][player->x] );
		map->fg[player->y][player->x] = SPACE;
		map->bg[player->y][player->x] = 0;
	}
}

#define MAX( a, b ) (a>b? (a):(b))
#define MIN( a, b ) (a<b? (a):(b))

/** Use "battle charge". */
void player_attack( int room_id, int player_id )
{
	/* TODO: labyrinth DFS */
	int x, y;
	int dmg_player_id;
	Player* dmg_player;
	Player* player = &( rooms.arr[room_id].players.arr[player_id] );
	Map* map = &( rooms.arr[room_id].map );
	for( y = MAX( player->y - 5, 1 ); y < MIN( player->y + 5, map->h - 2 ); ++y ) {
		for( x = MAX( player->x - 5, 1 ); x < MIN( player->x + 5, map->w - 2 ); ++x ) {
			/*printf( "%d %d\n", x, y );*/
			if( ISPLAYER( y, x )) {
				dmg_player_id = rooms.arr[room_id].map.pl[y][x];
				if( dmg_player_id != player_id ) {
					dmg_player = &( rooms.arr[room_id].players.arr[dmg_player_id] );
					player_damage( dmg_player, -game.hit_value );
				}

			}
		}
	}
}
