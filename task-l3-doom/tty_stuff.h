#ifndef TASK_L3__TTY_STUFF
#define TASK_L3__TTY_STUFF

#include "common_types.h"
#include <time.h>

int clear();
void set_canonical();
void restore();
int get_input( volatile int* type);

timer_t create_timer();
void start_timer(timer_t timerid, struct itimerspec* its );
void stop_timer(timer_t timerid);
void block_timer_signal();

#endif /* L3__TTY_STUFF */