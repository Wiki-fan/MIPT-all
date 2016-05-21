#ifndef TASK_L3__SERVER_ACTION_STUFF
#define TASK_L3__SERVER_ACTION_STUFF
#include "common_types.h"
/* How we distinct sockets of players and hosts */
#define ISHOST( sock ) (sock_info.arr[sock].player_id == -1)
#define ISUNINITIALIZED(sock) (sock_info.arr[sock].player_id == -2)
#define MARKHOST( info ) info.player_id = -1
#define MARKUNINITIALIZED( info ) info->player_id = -2

#define CHECKRESPONSE() \
if (resp == 0) {\
    info->pending_action = act;\
    return;\
} else if (resp == 1) {\
    info->pending_action = A_NULL;\
}

void close_room(int i);
void start_game(int i);
void send_rooms_list(int i);
void create_room(int i);
void join_room(int i);
void ask_players_list(int i);
void stop_game(int i);
void do_action(int i, int act);
void close_connection(int i);
void accept_connection();


#endif /* L3__SERVER_ACTION_STUFF */
