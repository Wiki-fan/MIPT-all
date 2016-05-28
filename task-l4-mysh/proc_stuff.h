#ifndef TASK_L4__PROC
#define TASK_L4__PROC

typedef struct
{
    /* in descriptor */
    int in;
    /* out descriptor */
    int out;
    /* count of arguments */
    int argc;
    /* pointer to string array of arguments */
    char** argv;
    /* pointers to in file and out file relatively (if corresponding descriptors are -1) */
    char *infile, *outfile;
    /* if should append to outfile*/
    int append;
    /* if should run in background */
    int backgr;
} proc;

int run(proc* fd, int fd_to_close);
void wait_and_display(int pid);

#endif /* TASK_L4__PROC */