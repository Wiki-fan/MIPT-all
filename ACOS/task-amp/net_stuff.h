#ifndef TASK_NET_STUFF
#define TASK_NET_STUFF
#include "common_types.h"

int send_buf( int sockfd, char* str );
int blocking_read_buf( int sockfd, char* buf );
void send_to_all( char* buf );

#endif /* TASK_NET_STUFF */
