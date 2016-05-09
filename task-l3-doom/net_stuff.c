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
extern Vector_SockIdInfo sock_info;

int send_buf( int sockfd, int count, char* buf )
{
	ssize_t n;
	int sent = 0;
	send_int(count, sockfd);

	while( sent < count ) {
		CN1( n = write( sockfd, buf + sent, count - sent ), E_WRITE );
		if (n == 0) {
			return -1;
		}
		sent += n;
	}
}

int send_int( int act, int sockfd )
{
	ssize_t n, sent = 0;

	while( sent < sizeof( int )) {
		CN1( n = write( sockfd, &act + sent, sizeof( int ) - sent ), E_WRITE );
		if (n == 0) {
			return -1;
		}
		sent += n;
	}
	return 0;
}

int blocking_read_int( int sockfd, int* ans )
{
	ssize_t n, recv = 0;
	while( recv < sizeof( int )) {
		CN1( n = read( sockfd, ans + recv, sizeof( int ) - recv ), E_READ );
		if (n == 0)
			return -1;
		recv += n;
	}

	return 1;
}

int blocking_read_buf( int sock_id, char* buf )
{
	int count, recv = 0, n;
	if (blocking_read_int(sockfd, &count) == -1)
		return -1;
	while( recv < count ) {
		CN1( n = read( sock_id, buf + recv, count - recv ), E_READ );
		if (n == 0)
			return -1;
		recv += n;
	}

	return 1;
}

/**Reads as many as can, returns 1, if read all, and 0, if not all.
 * Returns -1, if stream was closed. Exits with error on read error.
 */
int read_int( int sock_id, int* ans )
{
	ssize_t n;
	SockIdInfo* info = &sock_info.arr[sock_id];

	if (info->reading_what == READING_NOTHING) {
		info->readed = 0;
		info->needed = 0;
		info->reading_what = READING_INT;
		info->needed = sizeof( int );
	}
	CN1( n = read( sock_id, info->buf + info->readed, info->needed - info->readed ), E_READ );
	if (n == 0) { /* End of stream */
		return -1;
	}
	info->readed += n;
	if( info->readed == info->needed) {
		memcpy(ans, info->buf, sizeof(int));
		info->reading_what = READING_NOTHING;
		return 1;
	}
	return 0;
}

int read_buf( int sock_id, char* buf )
{
	SockIdInfo* info = &sock_info.arr[sock_id];
	ssize_t n, recv = 0;
	int resp;
	if (info->reading_what == READING_INT || info->reading_what == READING_NOTHING) {
		resp = read_int( sock_id, &info->needed );
		switch( resp ) {
			case -1:
				return -1; /* socket closed */
			case 0:
				return 0; /* partial read of int */
			case 1:
				break; /* whole int read */
			default:
				errx( 3, "Unreachable code" );
		}
	}

	if (info->reading_what != READING_BUF) {
		info->reading_what = READING_BUF;
		info->readed = 0;
	}
	CN1( n = read( sock_id, info->buf + info->readed, info->needed - info->readed ), E_READ );
	if (n == 0) { /* End of stream */
		return -1;
	}
	info->readed += n;
	if( info->readed == info->needed) {
		memcpy(buf, info->buf, info->needed); /* TODO: do we need it actually ? */
		info->readed = 0;
		info->needed = 0;
		info->reading_what = READING_NOTHING;
		return 1;
	}

	return 0;
}

int send_to_all_in_room( int room_id, int response )
{
	int i;

	for( i = 0; i < rooms.arr[room_id].players.size; ++i ) {
		LOG(( "Broadcasting message %d to %d socket", response, rooms.arr[room_id].players.arr[i].sock ));
		send_int( response, rooms.arr[room_id].players.arr[i].sock );
	}
}

