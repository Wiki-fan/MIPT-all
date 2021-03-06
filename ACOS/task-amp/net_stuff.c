#include "net_stuff.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <err.h>
#include <errno.h>
#include <sys/socket.h>
#include <unistd.h>

extern Vector_SockIdInfo sock_info;
extern Vector_Player players;

#define CNET(VAL, ERR)\
{\
	int ret;\
	while ( (ret = VAL) == -1) {\
        int error = errno;\
        if (error == EAGAIN)\
            continue;\
        if (error == ENOTCONN ) {\
            return -1;\
        } else {\
            perror("net failed");\
            exit(ERR);\
        }\
    }\
}

int send_buf( int sockfd, char* str )
{
    ssize_t n;
    int sent = 0;
    int count = strlen(str);
    while( sent < count )
    {
        CNET( n = write( sockfd, str + sent, count - sent ), E_WRITE );
        if (n == 0)
        {
            return -1;
        }
        sent += n;
    }
    return 1;
}

int blocking_read_buf( int sock_id, char* buf )
{
    int count=SERVER_BUF_SIZE, recv = 0;

    CNET( recv = read( sock_id, buf, count), E_READ );
    if (recv == -1 || recv == 0) {
        return -1;
    }
    if (recv > SERVER_BUF_SIZE-1) {
        err(E_BIGDATA, "Too big to fit in buffer");
    }

    return 1;
}

void send_to_all(char* str)
{
    int i;
    for( i = 0; i < players.size; ++i )
    {
        if (players.arr[i].sock != -1)
        {
            LOG(( "Broadcasting message to %d socket", players.arr[i].sock ));
            send_buf( players.arr[i].sock, str );
        }
    }
}
