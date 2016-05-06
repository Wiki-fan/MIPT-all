#ifndef TASK_L3__GAME_STUFF
#define TASK_L3__GAME_STUFF
#include "common_types.h"

void Vector_Room_add(Vector_Room* rooms, Room elem);

void GameQueue_init(GameQueue* q);
void GameQueue_push(GameQueue* q, SockIdInfo sock_info, enum ACTION act);
Node* GameQueue_pop(GameQueue* q);
void GameQueue_destroy(GameQueue* q);
int GameQueue_empty(GameQueue* q);

void player_move( int room_id, int player_id, int x, int y );
void player_init( Player* player, Map* map, char* name, int sock);
void player_mine( int room_id, int player_id );
void player_use( int room_id, int player_id );
void player_attack( int room_id, int player_id );

void player_kill(Player* player, Map* map);

void map_copy( Map* from, Map* to );
void map_delete(Map* map);

void room_delete(Room* room);

/* Player: fg==SPACE or BONUS, BG == bonus_value or undefined, pl=player_id
 * Bonus: fg==BONUS, bg==bonus value, pl==undefined
 * Mine: fg ==SPACE, bg==mine value, pl==miner id
 * Space: fg == SPACE, bg==0, pl==-1
 */

#define ISMINE(y, x) ( (map->fg[y][x] == SPACE \
	&& map->bg[y][x] != 0\
	&& map->pl[y][x] != player_id) ? 1:0 )
#define ISSPACE(y, x) ( (map->fg[y][x] == SPACE && map->pl[y][x] == -1) ? 1:0)
#define ISBONUS(y, x) ( (map->fg[y][x] == BONUS && (map->pl[y][x] == -1 || map->pl[y][x] == player_id)) ? 1:0)
#define ISPLAYER(y, x) ( ( ( (map->fg[y][x] == SPACE && map->bg[y][x] == 0) || map->fg[y][x] == BONUS)  && map->pl[y][x] != -1)  ? 1:0)
#define ISOURMINE(y, x) ( (map->fg[y][x] == SPACE \
	&& map->bg[y][x] != 0\
	&& map->pl[y][x] == player_id) ? 1:0 )

/* Map tile description */
#define BONUS '$'
#define WALL '#'
#define SPACE ' '
#define PLAYER '@'
#define YOU '%'
#define MINE '~'

#endif /* L3__GAME_STUFF */