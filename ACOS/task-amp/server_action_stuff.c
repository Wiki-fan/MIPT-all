#include "server_action_stuff.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <err.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include "net_stuff.h"

extern fd_set master;
extern fd_set read_fds;
extern int fdmax;
extern int listener_fd;
extern Vector_Player players;
extern Vector_SockIdInfo sock_info;
extern char buffer[SERVER_BUF_SIZE];
extern char buffer2[SERVER_BUF_SIZE];

char* skip_to_ws(char* buf) {
    char* actual_data = buf;
    while (*actual_data != ' ' && *actual_data != '\0' && *actual_data != '\n' && *actual_data != '\r') {
        ++actual_data;
    }
    *actual_data = '\0';
    ++actual_data;
    return actual_data;
}

size_t game_step = 0;

int check_if_player_exists(int i, char* name) {
    int j;
    int ans = 0;
    for (j = 0; j < players.size; ++j) {
        if (players.arr[j].sock != -1 && !strcmp(name, players.arr[j].name)) {
            ans = 1;
            break;
        }
    }
    if (ans == 0) {
        send_buf(i, "Such player don't exist\n");
    }
    return ans;
}

void perform_game_step() {
    int i;
    LOG(("Performing game step #%lu", game_step));
    ++game_step;

    /* Clearing flags and changing HP */
    for (i = 0; i < players.size; ++i) {
        Player* player = &players.arr[i];
        if (player->sock == -1) {
            continue;
        }
        player->already_healed = 0;
        player->already_attacked = 0;
        player->hp += player->delta_hp;
        if (player->hp > PLAYER_INITIAL_HP) {
            player->hp = PLAYER_INITIAL_HP;
        }
        player->delta_hp = 0;
        if (player->hp <= 0) {
            int j;
            LOG(("Player %d died", i));
            send_buf(player->sock, "You are killed\n");
            sprintf(buffer2, "%s is died\n", player->name);
            send_buf(players.arr[player->killer].sock, buffer2);
            for (j = 0; j < players.size; ++j) {
                if (players.arr[j].sock != -1 && j != player->killer && j != i) {
                    send_buf(players.arr[j].sock, "Somebody is dead. R.I.P.\n");
                }
            }
            close_connection(player->sock);
        }
    }
    send_to_all("Round finished\n");
}

void respond_who(int i) {

    int j;
    int count = 0;
    char buffer[1000];

    for (j = 0; j < players.size; ++j) {
        if (players.arr[j].sock != -1) {
            ++count;
        }
    }

    for (j = 0; j < players.size; ++j) {
        if (players.arr[j].sock != -1) {
            sprintf(buffer, "%s: %d hp\n", players.arr[j].name, players.arr[j].hp);
            send_buf(i, buffer);
        }
    }

    LOG(("Send list of %d players to socket %d", players.size, i));
}

void respond_wall(int shouter, char* buf) {
    memset(buffer2, 0, SERVER_BUF_SIZE);
    skip_to_ws(buf);
    sprintf(buffer2, "%s shouts: %s\n", players.arr[shouter].name, buf);
    send_to_all(buffer2);
}

void respond_say(int i, char* buf) {
    int j;
    char* str = skip_to_ws(buf);
    Player* sayer = &players.arr[sock_info.arr[i].player_id];
    skip_to_ws(str);

    if (!check_if_player_exists(i, buf)) {
        return;
    }

    memset(buffer2, 0, SERVER_BUF_SIZE);

    for (j = 0; j < players.size; ++j) {
        if (players.arr[j].sock != -1 && !strcmp(buf, players.arr[j].name)) {
            sprintf(buffer2, "%s say: %s\n", sayer->name, str);
            send_buf(players.arr[j].sock, buffer2);
            break;
        }
    }
}

void respond_kill(int i, char* buf) {
    int j;
    Player* attacker = &players.arr[sock_info.arr[i].player_id];

    skip_to_ws(buf);

    if (!check_if_player_exists(i, buf)) {
        return;
    }

    if (attacker->already_attacked) {
        send_buf(i, "You already killed in this round\n");
        return;
    }
    if (!strcmp(buf, attacker->name)) {
        send_buf(i, "Killing yourself is prohibited\n");
        return;
    }
    attacker->already_attacked = 1;

    for (j = 0; j < players.size; ++j) {
        if (players.arr[j].sock != -1 && !strcmp(buf, players.arr[j].name)) {
            Player* victim = &players.arr[j];
            victim->delta_hp -= rand() % MAX_KILL + 1;
            if (victim->hp + victim->delta_hp <= 0) {
                victim->killer = sock_info.arr[i].player_id;
            }
            sprintf(buffer2, "%s attacks you\n", attacker->name);
            send_buf(victim->sock, buffer2);
            break;
        }
    }
}

void respond_heal(int i, char* buf) {
    int j;
    Player* healer = &players.arr[sock_info.arr[i].player_id];

    skip_to_ws(buf);

    if (!check_if_player_exists(i, buf)) {
        return;
    }

    if (healer->already_healed == 1) {
        send_buf(i, "You already healed in this round\n");
        return;
    }
    healer->already_healed = 1;

    for (j = 0; j < players.size; ++j) {
        if (players.arr[j].sock != -1 && !strcmp(buf, players.arr[j].name)) {
            Player* victim = &players.arr[j];
            victim->delta_hp += rand() % MAX_HEAL + 1;
            break;
        }
    }
}

void close_connection(int i) {
    int player_id = sock_info.arr[i].player_id;
    /* connection closed*/
    close(i);
    FD_CLR(i, &master); /* remove from master set*/
    if (player_id != -1) {
        players.arr[player_id].sock = -1;
    }
    sock_info.arr[i].player_id = -1;

    LOG(("Socket %d hung up", i));
}

void accept_connection() {
    struct sockaddr_in cli_addr;
    int newsock_id;
    socklen_t clilen;
    SockIdInfo* info;

    clilen = sizeof(cli_addr);
    CN1(newsock_id = accept(listener_fd, (struct sockaddr*) &cli_addr, &clilen), E_ACCEPT);

    FD_SET(newsock_id, &master); /* add to master set*/
    if (newsock_id > fdmax)   /* keep track of the max*/
    {
        fdmax = newsock_id;
    }

    Vector_SockIdInfo_alloc(&sock_info, newsock_id);
    info = &sock_info.arr[newsock_id];
    info->sock_id = newsock_id;
    info->player_id = -1;

    LOG(("new connection: socket %d", newsock_id));

    send_buf(newsock_id, "Your login: ");
}
