#ifndef TASK_L3__TTY_STUFF
#define TASK_L3__TTY_STUFF
#include "common_types.h"
#include <time.h>
int clear();
void set_canonical();
void restore();
int get_input( int* type);
timer_t set_timer(struct itimerspec* its);

#endif /* L3__TTY_STUFF */