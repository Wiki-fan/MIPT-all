#include <signal.h>

#define REQUEST 1
#define RESPONSE 2

#define BUF_SIZE 100
typedef struct
{
    long mes_type;
    char buf[BUF_SIZE];
    pid_t client;
} mes_t;