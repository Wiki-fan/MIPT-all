#ifndef TASK_L3__TIMER_STUFF
#define TASK_L3__TIMER_STUFF

#include "common_types.h"
#include <time.h>

timer_t create_timer();
void start_timer(timer_t timerid, struct itimerspec* its );
void stop_timer(timer_t timerid);
void block_timer_signal();

#endif /* L3__TIMER_STUFF */
