#ifndef TASK_L3__NET_STUFF
#define TASK_L3__NET_STUFF
#include "common_types.h"

int server_init();

/* Following functions send (or receive) size of buffer in int, and then buffer */
void send_buf( int sockfd, int n, char* buf );
int read_buf(int sock_id, char* buf);
int send_int( int act, int sockfd );
int read_int( int sock_id );

#endif /* L3__NET_STUFF */
