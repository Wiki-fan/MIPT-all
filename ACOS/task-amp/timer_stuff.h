#ifndef TASK_TIMER_STUFF
#define TASK_TIMER_STUFF

#include "common_types.h"
#include <time.h>

timer_t create_timer();
void start_timer(timer_t timerid, struct itimerspec* its );
void stop_timer(timer_t timerid);
void block_timer_signal();

#endif /* TASK_TIMER_STUFF */
