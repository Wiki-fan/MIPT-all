#ifndef TASK_L4__PROC
#define TASK_L4__PROC

typedef struct {
	int in;
	int out;
	int err;
} stdfd;

int run(char* argv[], stdfd* fd);

#endif /* TASK_L4__PROC */