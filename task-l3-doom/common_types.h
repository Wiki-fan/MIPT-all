#ifndef TASK_L3__COMMON_TYPES
#define TASK_L3__COMMON_TYPES
#include "vector_impl.h"
#include "client_stuff.h"
#include "error_stuff.h"

#define NUM_OF_MINES 10
#define PORT 8024
#define BACKLOG 5
#define HOSTNAME "127.0.0.1"
#define FIELD_OF_SIGHT 10
#define INITIAL_NUM_OF_ROOMS 4
#define INITIAL_NUM_OF_PLAYERS 4
#define SERVER_BUF_SIZE 800

#define MAX_NAME_LEN 60
#define STR_MAX_NAME_LEN "60"

/* Global timer configuration */
#define SIG SIGALRM
#define CLOCKID CLOCK_REALTIME
#define NSEC_IN_SEC 1000000000

enum ACTION {
	A_UP,
	A_DOWN,
	A_LEFT,
	A_RIGHT,
	A_MINE,
	A_USE,
	A_ATTACK,
	A_NONE,
	A_EXIT,
	A_CREATE_ROOM,
	A_CLOSE_ROOM, /* close and delete, disconnect all players */
	A_ASK_ROOMS_LIST,
	A_JOIN_ROOM,
	A_START_GAME,
	A_STOP_GAME,
	A_ASK_PLAYER_LIST,
	A_NULL /* for server use only */
};

enum RESPONSE {
	R_ROOM_CREATED,
	R_SENDING_ROOMS,
	R_SENDING_PLAYERS,
	R_JOINED,
	R_DIED,
	R_DONE,
	R_GAME_STARTED, /* when host started game */
	R_GAME_FINISHED, /* when somebody won */
	R_GAME_STOPPED, /* by host */
	R_ROOM_CLOSED,
	R_CHEATER /* for suspiciously wrong actions */
};

typedef struct {
	char** fg; /* foreground: bonuses, walls, spaces */
	int** bg; /* background: mines, bonus hp delta */
	int** pl; /* Players id, -1 if no player. Information about mines owner.  */
	int w; /* width */
	int h; /*height */
} Map;

typedef struct {
	int initial_health;
	int hit_value;
	int recharge_duration;
	int mining_time;
	int stay_health_drop;
	int movement_health_drop;
	float step_standard_delay;
	int moratory_duration;
	Map map; /* map base, copy for every room */
} Game;

typedef struct {
	char name[MAX_NAME_LEN];
	int x, y;
	int num_of_mines;
	int hp;
	int sock;
} Player;
define_vector( Player )

typedef struct {
	char name[MAX_NAME_LEN];
	Vector_Player players;
	Map map;
	int is_started; /* 1, if game started, 0, if game stopped. */
	int is_exists; /* If room deleted (or not even created) is 0, else 1. */
	int left_alive; /* Number of live players */
	int host_sockid;
} Room;
define_vector( Room )

enum READINGWHAT {
	READING_NOTHING,
	READING_INT,
	READING_BUF
};

/* Information about room and player associated with every socket */
typedef struct {
	int room_id;
	int player_id;
	int sock_id;
	int reading_what;
	int read; /* We can read not all data from socket. If we do not, here the number of read bytes is stored. */
	int needed; /* Sometimes we read in buffer. Here we store buffer size if we have not read it all. */
	char buf[SERVER_BUF_SIZE];
	int pending_action;
} SockIdInfo;
define_vector( SockIdInfo )

struct SNode {
	struct SNode* next;
	SockIdInfo sock_info;
	int act;
};
typedef struct SNode Node;

typedef struct {
	Node* begin;
	Node* end;
} GameQueue;

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

#endif /* L3__COMMON_TYPES */
