#define _POSIX_C_SOURCE 200901L
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <err.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <err.h>
#include <sys/socket.h>
#include <netdb.h>
#include "../common/utils.h"
#include "../task-s2-gets_safe/safe_string.h"

int fd;
int setup_connection(char* hostname)
{
    int portno = 8080;
    struct sockaddr_in serv_addr;
    struct hostent* server;

    printf( "Connecting to host %s  port %d\n", hostname, portno );

    if (fd = socket( AF_INET, SOCK_STREAM, 0 ) == -1)
    {
        err(3, "socket");
    }

    if (server = gethostbyname( hostname ))
    {
        err(2, "ghbn");
    }

    memset((char*) &serv_addr, 0, sizeof( serv_addr ));
    serv_addr.sin_family = AF_INET;
    memcpy( &serv_addr.sin_addr.s_addr, server->h_addr_list[0], server->h_length );
    serv_addr.sin_port = htons((uint16_t) portno );

    /* Now connect to the server */
    if (connect( fd, (struct sockaddr*) &serv_addr, sizeof( serv_addr )) == -1)
        err(1, "connect");
}

/*int send_buf( int sockfd, int count, char* buf )
{
	ssize_t n;
	int sent = 0;
	send_int(count, sockfd);

	while( sent < count ) {
		CNET( n = write( sockfd, buf + sent, count - sent ), E_WRITE );
		if (n == 0) {
			return -1;
		}
		sent += n;
	}
	return 1;
}*/

#define HEADER "GET "
#define BUF_SIZE 100
int main(int argc, char* argv[])
{
    char* path, *request, *hostname, *filename;
    char buf[BUF_SIZE];
    int readed = 0;
    FILE* outf;
    if (argc != 3)
    {
        errx(4, "Wrong number of parameters");
    }
    hostname = argv[1];
    path = argv[2];
    request = malloc(sizeof(char)*(strlen(HEADER)+strlen(path)+2));
    strcpy(request, HEADER);
    strcpy(request+strlen(HEADER), path);
    printf("Request: %s\n", request);
    filename = path+strlen(path);
    while (*filename != '/')
    {
        --filename;
    }
    ++filename;
    printf("Filename: %s\n", filename);
    setup_connection(hostname);

    outf = fopen(filename, "w");

    while (1)
    {
        int curread = read(fd, buf, BUF_SIZE);
        if (curread == -1)
        {
            err(5, "read");
        }
        if (curread == 0)
        {
            fwrite(buf, sizeof(char), readed, outf);
            break;
        }
        if (readed < BUF_SIZE)
        {
            readed += curread;
        }
        if (readed == BUF_SIZE)
        {
            fwrite(buf, sizeof(char), readed, outf);
            readed = 0;
        }
    }
    free(request);
    fclose(outf);

    return 0;
}