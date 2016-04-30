#include <search.h>
#include <stdlib.h>
#include "game_stuff.h"
#include "config_stuff.h"
#include "net_stuff.h"
#include "tty_stuff.h"
#include "../common/utils.h"
#include "common_types.h"

void GameQueue_init(GameQueue* q)
{
	q->begin = q->end = NULL;
}

void GameQueue_push(GameQueue* q, SockIdInfo sock_info, enum ACTION act)
{
	Node* temp = malloc_s(sizeof(Node));
	temp->sock_info = sock_info;
	temp->act = act;
	temp->next = NULL;
	q->end->next = temp;
}

Node* GameQueue_pop(GameQueue* q)
{
	Node* temp = q->begin;
	q->begin = q->begin->next;
	return temp;
}

void GameQueue_destroy(GameQueue* q)
{
	Node* temp;
	while (q->begin != NULL) {
		temp = q->begin;
		q->begin = q->begin->next;
		free(temp);
	}
}
