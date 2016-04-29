#ifndef TASK_L3__COMMON_TYPES
#define TASK_L3__COMMON_TYPES

enum ACTION {
	A_UP, A_DOWN, A_LEFT, A_RIGHT, A_MINE, A_USE, A_ATTACK, A_EXIT
};

typedef struct
{
	int player_id;
	enum ACTION action;
} Action;

typedef struct {
	int initial_health;
	int hit_value;
	int recharge_duration;
	int mining_time;
	int stay_health_drop;
	int movement_health_drop;
	float step_standard_delay;
	int moratory_duration;
} Game;

typedef struct {
	char** m; /* map */
	int** b; /* background */
	int w; /* width */
	int h; /*height */
} Map;

typedef struct
{
	int x, y;
	int num_of_mines;
	int hp;
} Player;

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

#define NUM_OF_MINES 10
#define PORT 8084
#define BACKLOG 5
#define HOSTNAME "127.0.0.1"
#define FIELD_OF_SIGHT 10

#endif /* L3__COMMON_TYPES */