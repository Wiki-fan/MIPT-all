#include "game_stuff.h"
#include "config_stuff.h"

Game game;
Map map;
Player player;

void move(int x, int y)
{
	if (map.m[player.x + x][player.y+y] != WALL) {
		player.x += x;
		player.y += y;
	}
}

void set_mine()
{

}