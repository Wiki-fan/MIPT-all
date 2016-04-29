#ifndef TASK_L3__GAME_STUFF
#define TASK_L3__GAME_STUFF
#include "common_types.h"

void player_move(Player* player, int x, int y);

void player_init(Player* player);

void player_set_mine(Player* player);

void player_attack(Player* player);

/* map tile description */
#define BONUS '$'
#define WALL '#'
#define SPACE ' '
#define PLAYER '@'
#define MINE '~'

#endif /* L3__GAME_STUFF */