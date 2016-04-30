#include <search.h>
#include <stdlib.h>
#include <memory.h>
#include "game_stuff.h"
#include "config_stuff.h"
#include "net_stuff.h"
#include "tty_stuff.h"
#include "../common/utils.h"
#include "common_types.h"

extern Vector_Room rooms;
extern Game game;

void GameQueue_init(GameQueue* q)
{
	q->begin = q->end = NULL;
}

void GameQueue_push(GameQueue* q, SockIdInfo sock_info, enum ACTION act)
{
	Node* temp = malloc_s(sizeof(Node));
	temp->sock_info = sock_info;
	temp->act = act;
	temp->next = NULL;
	if (q->end == NULL) {
		q->begin = q->end = temp;
	} else {
		q->end->next = temp;
	}
}

Node* GameQueue_pop(GameQueue* q)
{
	Node* temp = q->begin;
	q->begin = q->begin->next;
	if (q->begin == NULL) {
		q->end = NULL;
	}
	return temp;
}

void GameQueue_destroy(GameQueue* q)
{
	Node* temp;
	while (q->begin != NULL) {
		temp = q->begin;
		q->begin = q->begin->next;
		free(temp);
	}
}

int GameQueue_empty(GameQueue* q)
{
	return (q->begin == NULL);
}

void copy_map(Map* from, Map* to)
{
	int i;
	to->h = from->h;
	to->w = from->w;
	to->fg = (char**) malloc_s( to->h * sizeof( char* ));
	to->bg = (int**) malloc_s( to->h * sizeof( int* ));
	for( i = 0; i < to->h; ++i ) {
		to->fg[i] = (char*) malloc_s( (to->w+1) * sizeof( char ));
		to->bg[i] = (int*) malloc_s( (to->w+1) * sizeof( int ));
		memcpy(to->fg[i], from->fg[i], to->w*sizeof(char));
		memcpy(to->bg[i], from->bg[i], to->w*sizeof(int));
	}
}

void player_move( int room_id, int player_id, int x, int y )
{
	Player* player = &( rooms.arr[room_id].players.arr[player_id] );
	if( rooms.arr[room_id].map.fg[player->y + y][player->x + x] != WALL ) {
		player->x += x;
		player->y += y;
	}
}

/*void player_init( int room_id, int player_id )
{
	Player* player = &( rooms.arr[room_id].players.arr[player_id] );
	player->x = player->y = 1;
	player->num_of_mines = NUM_OF_MINES;
	player->hp = game.initial_health;
}*/

void player_init( Player* player )
{
	player->x = player->y = 1;
	player->num_of_mines = NUM_OF_MINES;
	player->hp = game.initial_health;
}

void player_mine( int room_id, int player_id )
{
	Player* player = &( rooms.arr[room_id].players.arr[player_id] );
	Map* map = &( rooms.arr[room_id].map );
	if( map->fg[player->y][player->x] == SPACE ) {
		map->fg[player->y][player->x] = MINE;
		--player->num_of_mines;
	}
}

void player_use( int room_id, int player_id )
{
	Player* player = &( rooms.arr[room_id].players.arr[player_id] );
	Map* map = &( rooms.arr[room_id].map );
	if( map->fg[player->y][player->x] == BONUS ) {
		player->hp += map->bg[player->y][player->x];
	}
}

void player_attack( int room_id, int player_id )
{
	/* TODO: labyrinth DFS */
}
