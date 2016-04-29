#include "config_stuff.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <err.h>
#include "../common/utils.h"
#include "game_stuff.h"
#include "net_stuff.h"
#include "common_types.h"

extern Map map;
extern Game game;

#define BUF_SIZE 100

#define SWITCH if (0) {
#define CASEEQ( BUF, STR ) } else if(!strcmp(BUF, STR)) {
#define DEFAULT } else {
#define ENDSWITCH }

/*
enum CONFIG {
initial_health,
game.initial_health = (int)val;
hit_value,
game.hit_value = (int)val;
recharge_duration,
game.recharge_duration = (int)val;
mining_time,
game.mining_time = (int)val;
stay_health_drop,
game.stay_health_drop = (int)val;
movement_health_drop,
game.movement_health_drop = (int)val;
step_standard_delay,
game.step_standard_delay = val;
moratory_duration,
game.moratory_duration = (int)val;
DEFAULT
};*/

/** Read n characters into buffer buf which should store at least n+1 character for '\0' symbol */
int get_raw(FILE* f, char* buf, int n)
{
	int c;
	int i = 0;
	while ( (c = getc(f)) != EOF ) {
		if (c == '\n')
			break;
		buf[i++] = (char)c;
		if (i == n+1) {
			break;
		}
	}
	buf[i] = '\0';
	return i;
}

/** Reads config from file */
void read_config_from_file( char* file_name )
{
	FILE* inf;
	int x, y;
	float val;
	char buf[BUF_SIZE];
	LOG(("Starting to read config file\n"));
	inf = fopen_s( file_name, "r" );

	fscanf(inf, "%10s", buf);
	if( strcmp( buf, "Map" )) {
		errx( 20, "Map expected in config file" );
	}
	fscanf( inf, "%dx%d\n", &map.h, &map.w );
	/* Include borders in our map */
	map.w += 2;
	map.h += 2;

	map.m = (char**) malloc_s( map.h * sizeof( char* ));
	map.b = (int**) malloc_s( map.h * sizeof( int* ));
	for( y = 0; y < map.h; ++y ) {
		map.m[y] = (char*) malloc_s( (map.w+1) * sizeof( char ));
		map.b[y] = (int*) malloc_s( (map.w+1) * sizeof( int ));
		get_raw(inf, map.m[y], map.w);
	}

	while( get_raw( inf, buf, BUF_SIZE-1) != 0) {
		LOG(("Reading %s\n", buf));
		if( !strcmp( buf, "items:" )) {
			break;
		}
		fscanf( inf, " = %f\n", &val );
		SWITCH
		CASEEQ( buf, "initial_health" )
			game.initial_health = (int)val;
		CASEEQ( buf, "hit_value" )
			game.hit_value = (int)val;
		CASEEQ( buf, "recharge_duration" )
			game.recharge_duration = (int)val;
		CASEEQ( buf, "mining_time" )
			game.mining_time = (int)val;
		CASEEQ( buf, "stay_health_drop" )
			game.stay_health_drop = (int)val;
		CASEEQ( buf, "movement_health_drop" )
			game.movement_health_drop = (int)val;
		CASEEQ( buf, "step_standard_delay" )
			game.step_standard_delay = val;
		CASEEQ( buf, "moratory_duration" )
			game.moratory_duration = (int)val;
		DEFAULT
			errx( 21, "Unknown game parameter %s", buf );
		ENDSWITCH
	}

	while( fscanf( inf, "%d %d %f", &x, &y, &val ) != EOF) {
		map.m[y][x] = BONUS;
		map.b[y][x] = (int)val;
	}
	LOG(("Config read successfully\n"));

}