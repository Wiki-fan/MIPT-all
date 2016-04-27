#include "game_stuff.h"
#include "config_stuff.h"

#define NUM_OF_MINES 10
Game game;
Map map;
Player player;

void move(int x, int y)
{
	if (map.m[player.y+y][player.x + x] != WALL) {
		player.x += x;
		player.y += y;
	}
}

void player_init()
{
	player.x = player.y = 1;
	player.num_of_mines = NUM_OF_MINES;
	player.hp = game.initial_health;
}

void player_set_mine()
{
	if (map.m[player.y][player.x] == SPACE) {
		map.m[player.y][player.x] = MINE;
		--player.num_of_mines;
	}
}

void player_attack()
{

}