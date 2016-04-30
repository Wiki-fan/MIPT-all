#ifndef TASK_L3__NET_STUFF
#define TASK_L3__NET_STUFF
#include "common_types.h"

int server_init();

void GameQueue_init(GameQueue* q);
void GameQueue_push(GameQueue* q, SockIdInfo sock_info, enum ACTION act);
Node* GameQueue_pop(GameQueue* q);
void GameQueue_destroy(GameQueue* q);

#endif /* L3__NET_STUFF */
