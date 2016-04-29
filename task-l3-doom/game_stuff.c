#include <search.h>
#include "game_stuff.h"
#include "config_stuff.h"
#include "net_stuff.h"
#include "tty_stuff.h"
#include "../common/utils.h"
#include "common_types.h"

Game game;
Map map;
Player player;

struct Node {
	struct Node* next;
	int* player_num;
	enum ACTION act;
};

typedef struct {
	struct Node* begin;
	struct Node* end;
} GameQueue;

typedef struct {
	Player* player_array;
	size_t size;
	size_t max_size;
} Vector;

void Vector_init(Vector* v, size_t init_size)
{
	v->player_array = malloc_s(init_size*sizeof(Player));
	v->max_size = init_size;
	v->size = 0;
}
void Vector_push(Vector* v, Player* elem)
{

}

void player_move(Player* player, int x, int y)
{
	if (map.m[player->y+y][player->x + x] != WALL) {
		player->x += x;
		player->y += y;
	}
}

void player_init(Player* player)
{
	player->x = player->y = 1;
	player->num_of_mines = NUM_OF_MINES;
	player->hp = game.initial_health;
}

void player_set_mine(Player* player)
{
	if (map.m[player->y][player->x] == SPACE) {
		map.m[player->y][player->x] = MINE;
		--player->num_of_mines;
	}
}

void player_attack(Player* player)
{
 /* TODO: labyrinth DFS */
}