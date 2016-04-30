#ifndef TASK_L3__COMMON_TYPES
#define TASK_L3__COMMON_TYPES
#include "vector_impl.h"

enum ACTION {
	A_UP, A_DOWN, A_LEFT, A_RIGHT, A_MINE, A_USE, A_ATTACK, A_EXIT
};

typedef struct
{
	int player_id;
	enum ACTION action;
} Action;

typedef struct {
	char** m; /* map */
	int** b; /* background */
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
	Map* map; /* map base, copy for every room */
} Game;

typedef struct
{
	int x, y;
	int num_of_mines;
	int hp;
} Player;

/* Information about room and player associated with every socket */
typedef struct {
	int room_id;
	int player_id;
} SockIdInfo;

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

#define LOG(params ) printf params

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

#define NUM_OF_MINES 10
#define PORT 8085
#define BACKLOG 5
#define HOSTNAME "127.0.0.1"
#define FIELD_OF_SIGHT 10
#define INITIAL_NUM_OF_ROOMS 4
#define INITIAL_NUM_OF_PLAYERS 4

#endif /* L3__COMMON_TYPES */