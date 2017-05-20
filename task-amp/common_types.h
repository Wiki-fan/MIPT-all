#ifndef TASK_L3__COMMON_TYPES
#define TASK_L3__COMMON_TYPES
#include "vector_impl.h"
#include "error_stuff.h"


#define BACKLOG 5
#define SERVER_BUF_SIZE 800
#define MAX_HEAL 10
#define MAX_KILL 10

#define MAX_NAME_LEN 10
#define PLAYER_INITIAL_HP 10

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

/* Information about room and player associated with every socket */
typedef struct
{
    int player_id;
    int sock_id;
} SockIdInfo;
define_vector( SockIdInfo )

#define LOG( params ) printf params ; putchar('\n')

/*#define debug(smth) #ifdef DEBUG\
printf smth \
#endif*/

#define CH0( COMM, CODE, MESSAGE )\
if ((COMM) == NULL) \
err(CODE, MESSAGE);

#define CHN0( COMM, CODE, MESSAGE )\
if ((COMM) != 0) \
err(CODE, MESSAGE);

/** Because we can, our min-max macro */
#define MAX( a, b ) (a>b? (a):(b))
#define MIN( a, b ) (a<b? (a):(b))

extern int port;

#endif /* L3__COMMON_TYPES */
