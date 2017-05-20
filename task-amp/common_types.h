#ifndef TASK_COMMON_TYPES
#define TASK_COMMON_TYPES
#include "vector_impl.h"
#include "error_handling.h"


#define BACKLOG 5
#define SERVER_BUF_SIZE 800
#define MAX_HEAL 10
#define MAX_KILL 10

#define MAX_NAME_LEN 10
#define PLAYER_INITIAL_HP 100
#define SECONDS_IN_ROUND 20

/* Global timer configuration */
#define SIG SIGALRM
#define CLOCKID CLOCK_REALTIME

typedef struct
{
    char name[MAX_NAME_LEN+1];
    int hp;
    int already_attacked, already_healed;
    int sock;
    int delta_hp;
    int killer;
} Player;
define_vector( Player )

/* Information about player associated with every socket */
typedef struct
{
    int player_id;
    int sock_id;
} SockIdInfo;
define_vector( SockIdInfo )

#define LOG( params ) printf params ; putchar('\n')

extern int port;

#endif /* TASK_COMMON_TYPES */
