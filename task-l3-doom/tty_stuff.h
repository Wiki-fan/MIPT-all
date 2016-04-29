#ifndef TASK_L3__TTY_STUFF
#define TASK_L3__TTY_STUFF
#include "common_types.h"

void set_canonical();
void restore();
int get_input(enum ACTION* type);
void render();

#endif /* L3__TTY_STUFF */