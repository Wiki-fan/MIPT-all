#ifndef TASK_L3__COMMON_TYPES
#define TASK_L3__COMMON_TYPES
#include "vector_impl.h"

#define NUM_OF_MINES 10
#define PORT 8093
#define BACKLOG 5
#define HOSTNAME "127.0.0.1"
#define FIELD_OF_SIGHT 10
#define INITIAL_NUM_OF_ROOMS 4
#define INITIAL_NUM_OF_PLAYERS 4

#define MAX_NAME_LEN 60
#define STR_MAX_NAME_LEN "60"

enum ACTION {
	A_UP,
	A_DOWN,
	A_LEFT,
	A_RIGHT,
	A_MINE,
	A_USE,
	A_ATTACK,
	A_EXIT,
	A_CREATEROOM,
	A_ASK_ROOMS_LIST,
	A_JOINROOM,
	A_STARTGAME,
	A_ASK_PLAYER_LIST
};

enum RESPONSE {
	R_CREATED,
	R_SENDING_ROOMS,
	R_SENDING_PLAYERS,
	R_JOINED,
	R_DIED,
	R_DONE
};

typedef struct {
	char** fg;
	/* foreground: bonuses, walls, spaces */
	int** bg;
	/* background: mines, bonus hp delta */
	int** pl;
	/* Players id, -1 if no player. Information about mines owner.  */
	int w;
	/* width */
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
	Map* map; /* map base, copy for every room */
} Game;

typedef struct {
	char name[MAX_NAME_LEN];
	int x, y;
	int num_of_mines;
	int hp;
} Player;
define_vector( Player )

typedef struct {
	char name[MAX_NAME_LEN];
	Vector_Player players;
	Map map;
} Room;
define_vector( Room )

/* Information about room and player associated with every socket */
typedef struct {
	int room_id;
	int player_id;
	int sock_id;
} SockIdInfo;
define_vector( SockIdInfo )

struct SNode {
	struct SNode* next;
	SockIdInfo sock_info;
	enum ACTION act;
};
typedef struct SNode Node;

typedef struct {
	Node* begin;
	Node* end;
} GameQueue;

#define LOG( params ) printf params

/*#define debug(smth) #ifdef DEBUG\
printf smth \
#endif*/

#define CHN1( COMM, CODE, MESSAGE )\
if ((COMM) == -1) \
err(CODE, MESSAGE);

#define CH0( COMM, CODE, MESSAGE )\
if ((COMM) == NULL) \
err(CODE, MESSAGE);

#define CHN0( COMM, CODE, MESSAGE )\
if ((COMM) != 0) \
err(CODE, MESSAGE);

#endif /* L3__COMMON_TYPES */