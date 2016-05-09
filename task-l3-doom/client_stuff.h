#ifndef TASK_L3__CLIENT_STUFF
#define TASK_L3__CLIENT_STUFF

#include "common_types.h"
#define CLIENT_BUF_SIZE 800
#define STR_CLIENT_BUF_SIZE "800"

int setup_connection();
int ask_player_or_host();
void ask_room_name( char* buf );
void ask_player_name( char* buf );
int ask_host_action();
int ask_which_room( int room_count );
int getrecvlist( );
int getplayerlist(  );
void render();

#define CHK_RESPONSE( NEEDED, MSG )\
{\
	int ans;\
	if (blocking_read_int(sockfd, &ans) == -1) \
	    errx(23, "Lost connection to server");\
	if (ans == NEEDED) {\
	printf(MSG); putchar('\n');\
	} else {\
	errx(13, "Wrong response");\
	}\
}

extern char buf[CLIENT_BUF_SIZE];
extern int sockfd;

#endif /* L3__CLIENT_STUFF */
