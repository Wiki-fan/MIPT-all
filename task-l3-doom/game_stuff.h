#ifndef TASK_L3__GAME_STUFF
#define TASK_L3__GAME_STUFF
#include "common_types.h"

void GameQueue_init(GameQueue* q);
void GameQueue_push(GameQueue* q, SockIdInfo sock_info, enum ACTION act);
Node* GameQueue_pop(GameQueue* q);
void GameQueue_destroy(GameQueue* q);
int GameQueue_empty(GameQueue* q);

void player_move( int room_id, int player_id, int x, int y );
/*void player_init( int room_id, int player_id );*/
void player_init( Player* player );
void player_mine( int room_id, int player_id );
void player_use( int room_id, int player_id );
void player_attack( int room_id, int player_id );

void copy_map(Map* from, Map* to);

/* map tile description */
#define BONUS '$'
#define WALL '#'
#define SPACE ' '
#define PLAYER '@'
#define MINE '~'

#endif /* L3__GAME_STUFF */