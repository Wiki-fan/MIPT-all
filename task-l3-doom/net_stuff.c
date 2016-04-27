#include "net_stuff.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <err.h>
#include <memory.h>
int port_num;
char* host_name;
int sock_id;

struct hostent *hst;
struct sockaddr_in server_addr;

int setup_connection(int port_num, char* hostname)
{
	int sock_id;

	/* setup socket */
	sock_id = socket(AF_UNIX, SOCK_STREAM, 0);

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons((uint16_t)port_num);
	memcpy(&(server_addr.sin_addr.s_addr),
	       hst->h_addr_list[0], hst->h_length);

	if (connect(sock_id, (struct sockaddr*)(&server_addr),
	            sizeof(struct sockaddr_in)) == -1)
	{
		err(28, "Connection failed");
	}

	send(sock_id, "Hello", 4, 0);
	return 0;

}

#define NUM_SIMULT_CONN_CLIENTS 5
int setup_server()
{
	int sock_id;
	int port_num;
	int bind_ret;
	int acc_ret;
	struct sockaddr_in server_addr;

	port_num = atoi(argv[1]);
	sock_id = socket(PF_INET, SOCK_STREAM, 0);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons((short)port_num);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	bind_ret = bind(sock_id, (struct sockaddr*)(&server_addr), sizeof(struct sockaddr_in));
	if (bind_ret == -1)
	{
		err(29, "Bind failed");
	}
	listen(sock_id, NUM_SIMULT_CONN_CLIENTS);
}