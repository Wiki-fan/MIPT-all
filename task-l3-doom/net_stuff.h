  #ifndef TASK_L3__NET_STUFF
#define TASK_L3__NET_STUFF
#include "common_types.h"

int server_start();

/** Following functions send (or receive) size of buffer in int, and then buffer
 * To do non-blocking read, read functions will read only what they can, filling the buffer.
 * If read finished successfully, they will return 1, else 0.
 */
int send_buf( int sockfd, int n, char* buf );
int read_buf(int sock_id, char* buf);
int send_int( int act, int sockfd );
int read_int( int sock_id, int* ans );
int blocking_read_int( int sockfd, int* ans );
int blocking_read_buf( int sockfd, char* buf );
int send_to_all_in_room(int room_id, int response);

#endif /* L3__NET_STUFF */
