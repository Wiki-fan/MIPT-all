#include "game_stuff.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include "../common/utils.h"
#include "common_types.h"

extern Vector_Room rooms;
extern Game game;
extern Vector_SockIdInfo sock_info;
extern fd_set master;

/** Add room to first free place in room vector. */
int Vector_Room_add( Vector_Room* rooms, Room elem )
{
    int i;
    for( i = 0; i < rooms->size; ++i )
    {
        if( rooms->arr[i].is_exists == 0 )
        {
            rooms->arr[i] = elem;
            return i;
        }
    }
    Vector_Room_push( rooms, elem );
    return rooms->size - 1;
}

void GameQueue_init( GameQueue* q )
{
    q->begin = q->end = NULL;
}

void GameQueue_push( GameQueue* q, SockIdInfo sock_info, int act )
{
    Node* temp = malloc_s( sizeof( Node ));
    temp->sock_info = sock_info;
    temp->act = act;
    temp->next = NULL;
    if( q->end == NULL)
    {
        q->begin = q->end = temp;
    }
    else
    {
        q->end->next = temp;
    }
}

Node* GameQueue_pop( GameQueue* q )
{
    Node* temp = q->begin;
    q->begin = q->begin->next;
    if( q->begin == NULL)
    {
        q->end = NULL;
    }
    return temp;
}

void GameQueue_destroy( GameQueue* q )
{
    Node* temp;
    while( q->begin != NULL)
    {
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
    to->splash = (char**) malloc_s(to->h * sizeof(char*));
    for( i = 0; i < to->h; ++i )
    {
        to->fg[i] = (char*) malloc_s(( to->w ) * sizeof( char ));
        to->bg[i] = (int*) malloc_s(( to->w ) * sizeof( int ));
        to->pl[i] = (int*) malloc_s(( to->w ) * sizeof( int ));
        to->splash[i] = (char*) malloc_s(( to->w ) * sizeof( char ));
        memcpy( to->fg[i], from->fg[i], to->w * sizeof( char ));
        memcpy( to->bg[i], from->bg[i], to->w * sizeof( int ));
        for( j = 0; j < to->w; ++j )
        {
            to->pl[i][j] = -1;
            to->splash[i][j] = -1;
        }
    }
}

void map_delete( Map* map )
{
    int i;
    for( i = 0; i < map->h; ++i )
    {
        free( map->fg[i] );
        free( map->bg[i] );
        if( map->pl != NULL)
        {
            free( map->pl[i] );
        }
        if( map->splash != NULL)
        {
            free( map->splash[i] );
        }
    }
    free( map->fg );
    free( map->bg );
    if( map->pl != NULL)
    {
        free( map->pl );
    }
    if( map->splash != NULL)
    {
        free( map->splash );
    }
}

/** Delete room, free map and player vector, mark room as nonexistent. */
void room_delete( Room* room )
{
    room->is_exists = 0;
    map_delete( &(room->map) );
    Vector_Player_destroy( &room->players );
}

/** Change player's health on value points. Returns 0, if player was killed. */
int player_damage( Player* player, int value )
{
    player->hp += value;
    return player->hp > 0;
}

/** Kill player and move it out of map. */
void player_kill( Player* player, Map* map )
{
    if( player->y == -1 )
    {
        return;
    }
    map->pl[player->y][player->x] = -1;
    player->x = -1;
    player->y = -1;
    player->hp = -1;
}

/* Close player's socket. Send him that he died before calling function. */
void player_close_socket( Player* player )
{
    close( sock_info.arr[player->sock].sock_id );
    FD_CLR( player->sock, &master ); /* remove from master set*/
    player->sock = -1;
}

/** Move player on (x, y). */
void player_move( int room_id, int player_id, int x, int y )
{
    Player* player = &( rooms.arr[room_id].players.arr[player_id] );
    Map* map = &rooms.arr[room_id].map;
    if( player->mining > 0 )
    {
        return;
    }
    player->is_moving = 1;

    /* if we go to bonus or space, just move */
    if( ISSPACE( player->y + y, player->x + x ) || ISBONUS( player->y + y, player->x + x ) || ISOURMINE( player->y + y, player->x + x ))
    {
        if( !ISOURMINE( player->y, player->x ))
        {
            map->pl[player->y][player->x] = -1;
        }
        player->x += x;
        player->y += y;
        map->pl[player->y][player->x] = player_id;
    }
    else   /* if we stepped on mine, and mine is not our mine */
        if( ISMINE( player->y + y, player->x + x ))
        {
            map->pl[player->y][player->x] = -1;
            player->x += x;
            player->y += y;
            player_damage( player, map->bg[player->y][player->x] );
            map->bg[player->y][player->x] = 0;
            map->pl[player->y][player->x] = player_id;
            if (player->hp < 0)
            {
                player_kill(player, map);
            }

        }
}

/** Initial player's parameters. Initializes barely game stuff, not sock or something. Init map on your own, too. */
void player_init( Player* player, Map* map )
{
    /* Place player randomly */
    int fl = 1;
    while( fl )
    {
        player->x = rand() % ( game.map.w - 2 ) + 1;
        player->y = rand() % ( game.map.h - 2 ) + 1;
        if( ISSPACE( player->y, player->x ))
        {
            fl = 0;
        }
    }

    player->num_of_mines = NUM_OF_MINES;
    player->hp = game.initial_health;
    player->cooldown = 0;
    player->mining = 0;
    player->is_moving = 0;
}

/** Set mine on this tile */
void player_mine( int room_id, int player_id )
{
    Player* player = &( rooms.arr[room_id].players.arr[player_id] );
    Map* map = &( rooms.arr[room_id].map );
    if( player->mining > 0 )
    {
        return;
    }
    player->mining = game.mining_time;
    if( player->num_of_mines > 0 && map->fg[player->y][player->x] == M_SPACE
            && !ISOURMINE( player->y, player->x ))   /* We can't place mine on bonus or other mine */
    {
        /*map->fg[player->y][player->x] = M_MINE;*/
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
    if( ISBONUS( player->y, player->x ))
    {
        player_damage( player, map->bg[player->y][player->x] );
        map->fg[player->y][player->x] = M_SPACE;
        map->bg[player->y][player->x] = 0;
    }
}

#define CHECK_SPLASH(dx, dy) \
if (!ISWALL(y+ dy, x+ dx) && map->splash[y+ dy][x+ dx] == -1) {\
	map->splash[y+ dy][x+ dx] = k+1;\
}

/** Use "battle charge". */
void player_attack( int room_id, int player_id )
{
    int x, y;
    int k;
    int dmg_player_id;
    Player* dmg_player;
    Player* player = &( rooms.arr[room_id].players.arr[player_id] );
    Map* map = &( rooms.arr[room_id].map );
    if( player->cooldown > 0 )
    {
        return;
    }
    player->cooldown = game.recharge_duration;
    map->splash[player->y][player->x] = 0;
    for( k = 0; k < BATTLE_CHARGE_RADIUS; ++k )
    {
        for( y = MAX( player->y - BATTLE_CHARGE_RADIUS, 1 ); y < MIN( player->y + BATTLE_CHARGE_RADIUS, map->h - 1 ); ++y )
        {
            for( x = MAX( player->x - BATTLE_CHARGE_RADIUS, 1 ); x < MIN( player->x + BATTLE_CHARGE_RADIUS, map->w - 1 ); ++x )
            {
                /*printf( "%d %d\n", x, y );*/
                if (map->splash[y][x] == k)
                {
                    CHECK_SPLASH(-1, 0)
                    CHECK_SPLASH(1, 0)
                    CHECK_SPLASH(0, 1)
                    CHECK_SPLASH(0, -1)
                }
            }
        }
    }
    for( y = MAX( player->y - BATTLE_CHARGE_RADIUS-1, 0 ); y < MIN( player->y + BATTLE_CHARGE_RADIUS+1, map->h ); ++y )
    {
        for( x = MAX( player->x - BATTLE_CHARGE_RADIUS-1, 0 ); x < MIN( player->x + BATTLE_CHARGE_RADIUS+1, map->w ); ++x )
        {
            if( map->splash[y][x] != -1 )
            {
                /*printf("%2d", map->splash[y][x]);*/
                if( ISPLAYER( y, x ))
                {
                    dmg_player_id = rooms.arr[room_id].map.pl[y][x];
                    if( dmg_player_id != player_id )
                    {
                        dmg_player = &( rooms.arr[room_id].players.arr[dmg_player_id] );
                        player_damage( dmg_player, -game.hit_value );
                        if (player->hp < 0)
                        {
                            player_kill(player, map);
                        }
                    }
                }
                map->splash[y][x] = -1;
            }
            else
            {
                /*printf("##");*/
            }
        }
        /*printf("\n");*/
    }
}
