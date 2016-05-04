#include "net_stuff.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <err.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <netdb.h>
#include <unistd.h>
#include <strings.h>
#include <pthread.h>
#include <unitypes.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include "../common/utils.h"
#include "game_stuff.h"
#include "tty_stuff.h"
#include "config_stuff.h"
#include "common_types.h"
#include "data_stuff.h"

extern Vector_Room rooms;

int read_buf( int sock_id, char* buf )
{
	int n, count, recv;
	recv = 0;
	while (recv < sizeof(int)) {
		CHN1( n = read( sock_id, &count, sizeof( int )-recv), 22, "Can't read the socket" );
		recv += n;
	}
	recv = 0;
	while (recv < count) {
		CHN1( n = read( sock_id, buf, count-recv ), 22, "Can't read the socket" );
		recv += n;
	}

	return count;
}

void send_buf( int sockfd, int count, char* buf )
{
	int n, sent;
	sent = 0;
	while (sent < sizeof(int)) {
		CHN1( n = write( sockfd, &count, sizeof( int )-sent), 27, "Socket write error" );
		sent += n;
	}
	sent = 0;
	while(sent < count) {
		CHN1( n = write( sockfd, buf, count -sent), 27, "Socket write error" );
		sent += n;
	}
}

int send_int( int act, int sockfd )
{
	int n, sent = 0;

	while (sent < sizeof(int)) {
		CHN1( n = write( sockfd, &act, sizeof( int )-sent), 27, "Socket write error" );
		sent += n;
	}
	return 0;
}

int read_int( int sock_id )
{
	int n, recv = 0, ret;
	while (recv < sizeof(int)) {
		CHN1( n = read( sock_id, &ret, sizeof( int )-recv), 22, "Can't read the socket" );
		recv += n;
	}
	return ret;
}

int send_to_all_in_room(int room_id, int response) {
	int i;

	for (i = 0; i<rooms.arr[room_id].players.size; ++i) {
		LOG(("Broadcasting message to %d socket", rooms.arr[room_id].players.arr[i].sock));
		send_int(response, rooms.arr[room_id].players.arr[i].sock);
	}
}

