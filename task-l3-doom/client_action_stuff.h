#ifndef TASK_L3__CLIENT_ACTION_STUFF
#define TASK_L3__CLIENT_ACTION_STUFF
#include "common_types.h"

enum GAME {
	G_CONTINUE, G_RESTART, G_EXIT, G_CHANGE_ROOM, G_WAIT_END
};

void host();
void play();

#endif /* L3__CLIENT_ACTION_STUFF */
