#ifndef TASK_L3__GAME_STUFF
#define TASK_L3__GAME_STUFF

typedef struct
{
	int x, y;
	int num_of_mines;
	int hp;
} Player;

void move(int x, int y);
void player_init();
void player_set_mine();

/* map tile description */
#define BOOST '$'
#define WALL '#'
#define SPACE ' '
#define PLAYER '@'
#define MINE '~'
/*169*/

#endif /* L3__GAME_STUFF */