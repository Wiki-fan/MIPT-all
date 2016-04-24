#ifndef TASK_L3__CONFIG_STUFF
#define TASK_L3__CONFIG_STUFF

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

void read_config_from_file( char* file_name );

#endif /* L3__CONFIG_STUFF */