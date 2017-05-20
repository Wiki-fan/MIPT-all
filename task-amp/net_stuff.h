#ifndef TASK_L3__NET_STUFF
#define TASK_L3__NET_STUFF
#include "common_types.h"

int server_start();

/** Following functions send (or receive) size of buffer in int, and then buffer
 * To do non-blocking read, read functions will read only what they can, filling the buffer.
 * If read finished successfully, they will return 1, else 0.
 */
int send_buf( int sockfd, char* str );
int blocking_read_buf( int sockfd, char* buf );
void send_to_all( char* buf );

#endif /* L3__NET_STUFF */
