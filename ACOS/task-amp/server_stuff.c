#define _POSIX_C_SOURCE 200901L
#include "server_stuff.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <err.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <errno.h>
#include "timer_stuff.h"
#include "net_stuff.h"
#include "server_action_stuff.h"

/* master file descriptor list*/
fd_set master;
/* temp file descriptor list for select()*/
fd_set read_fds;
/* max num of file descriptor, so we check only [0, fdmax] */
int fdmax;

int listener_fd, port_num;
timer_t timerid;

int server_work = 1;
int exitcode = 0;

char buffer[SERVER_BUF_SIZE];
char buffer2[SERVER_BUF_SIZE];

Vector_SockIdInfo sock_info;
Vector_Player players;

/** Initial player's parameters. Initializes barely game stuff, not sock or something. */
void player_init(Player* player) {
    player->hp = PLAYER_INITIAL_HP;
    player->delta_hp = 0;
    player->already_attacked = 0;
    player->already_healed = 0;
    player->killer = -1;
}

/** Loop where network input is processed. */
void* server_loop() {
    int i;

    while (server_work) {
        read_fds = master;
        if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1) {
            int error = errno;
            if (error == EINTR) {
                continue;
            } else {
                err(error, "%s", errmsg[error]);
            }
        }

        for (i = 0; i <= fdmax; i++) {
            if (FD_ISSET(i, &read_fds)) {
                /* accept new connection */
                if (i == listener_fd) {
                    accept_connection();

                } else {
                    char* message = buffer;
                    int resp;
                    /* handle data from a client*/
                    resp = blocking_read_buf(i, message);

                    if (resp == 0) {
                        continue;
                    } else if (resp == -1) {
                        close_connection(i);
                    } else {
                        int player_id = sock_info.arr[i].player_id;
                        /*LOG(("Socket %d sends: %s", i, message));*/

                        if (player_id == -1) {
                            skip_to_ws(message);
                            LOG(("Proposed login: %s", message));
                            if (strlen(message) > MAX_NAME_LEN) {
                                send_buf(i, "Login must not be longer than 10 characters\n");
                                continue;
                            }

                            {
                                char* c;
                                int fl = 1;
                                for (c = message; *c != '\0'; ++c) {
                                    if (!((*c >= 'a' && *c <= 'z') || (*c >= 'A' && *c <= 'Z'))) {
                                        send_buf(i, "Login must be [a-zA-Z]\n");
                                        fl = 0;
                                        break;
                                    }
                                }
                                if (fl == 0) {
                                    continue;
                                }
                            }

                            {
                                int j;
                                int fl = 1;
                                for (j = 0; j<players.size; ++j) {
                                    if (players.arr[j].sock != -1 && !strcmp(message, players.arr[j].name)) {
                                        send_buf(i, "We already have this login in the server\n");
                                        fl = 0;
                                        break;
                                    }
                                }
                                if (fl == 0) {
                                    continue;
                                }
                            }

                            {

                                Player player;
                                SockIdInfo* info = &sock_info.arr[i];

                                strncpy(player.name, message, MAX_NAME_LEN+1);
                                skip_to_ws(player.name);
                                player.sock = i;
                                player_init(&player);

                                info->player_id = players.size;
                                info->sock_id = i;
                                Vector_Player_push(&players, player);
                                send_buf(i, "Welcome to the game\n");
                            }
                        } else {
                            char* actual_data = skip_to_ws(message);
                            if (!strcmp(message, "who")) {
                                respond_who(i);
                            } else if (!strcmp(message, "wall")) {
                                respond_wall(sock_info.arr[i].player_id, actual_data);
                            } else if (!strcmp(message, "say")) {
                                respond_say(i, actual_data);
                            } else if (!strcmp(message, "kill")) {
                                respond_kill(i, actual_data);
                            } else if (!strcmp(message, "heal")) {
                                respond_heal(i, actual_data);
                            } else {
                                LOG(("Unknown command %s", message));
                                send_buf(i, "Unknown command\n");
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}

void server_cleanup() {
    stop_timer(timerid);
    LOG(("Starting cleanup"));
    Vector_SockIdInfo_destroy(&sock_info);
    Vector_Player_destroy(&players);
    LOG(("Vectors destroyed"));
    timer_delete(timerid);
    LOG(("Timer deleted"));
}

static void handler(int sig) {
    /*printf("Enter handler\n");*/
    switch (sig) {
        case SIGTERM:LOG(("Exiting on SIGTERM"));
            server_cleanup();
            exitcode = 0; /*SIGTERM;*/
            server_work = 0;
            break;
        case SIGINT:LOG(("Exiting on SIGINT"));
            server_cleanup();
            exitcode = SIGINT;
            server_work = 0;
            break;
        case SIGALRM:perform_game_step();
            break;
        case SIGUSR1:break;
        default:err(3, "Unreachable code");
    }
}

/** Init server (open socket, bind, listen, spawn threads) */
int server_start() {
    struct sockaddr_in serv_addr;
    struct sigaction sa;
    struct itimerspec its;

    LOG(("Server started"));

    port_num = port;

    CN1(listener_fd = socket(AF_INET, SOCK_STREAM, 0), E_SOCKET);

    /* initialize socket structure */
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons((uint16_t) port_num);

    /* bind the socket*/
    CN1(bind(listener_fd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)), E_BIND);

    /* listen socket */
    CN1(listen(listener_fd, BACKLOG), E_LISTEN);

    /* initialize fd_set structure */
    FD_ZERO(&master);
    FD_SET(listener_fd, &master);
    fdmax = listener_fd;

    block_timer_signal();

    /* Init timer parameters */
    its.it_interval.tv_sec = SECONDS_IN_ROUND;
    its.it_interval.tv_nsec = 0;
    its.it_value = its.it_interval;

    /* Setting signal handler */
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handler;
    sigfillset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    CN1(sigaction(SIGTERM, &sa, NULL), E_SIGACTION);
    CN1(sigaction(SIGINT, &sa, NULL), E_SIGACTION);
    CN1(sigaction(SIGALRM, &sa, NULL), E_SIGACTION);

    /* Start timer */
    start_timer(timerid, &its);

    Vector_Player_init(&players, 10);
    Vector_SockIdInfo_init(&sock_info, 10);
    server_loop();
    return 0;
}
