#ifndef TASK_SERVER_ACTION_STUFF
#define TASK_SERVER_ACTION_STUFF
#include "common_types.h"

char* skip_to_ws(char* buf);

void respond_who(int i);
void respond_wall(int shouter, char* buf);
void respond_say(int i, char* buf);
void respond_kill(int i, char* buf);
void respond_heal(int i, char* buf);
void close_connection(int i);
void accept_connection();


#endif /* TASK_SERVER_ACTION_STUFF */
