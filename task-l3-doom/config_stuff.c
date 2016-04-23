#include "config_stuff.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <err.h>
#include "../common/utils.h"
#include "game_stuff.h"

extern Map map;
extern Game game;

#define BUF_SIZE 100

#define SWITCH if (0) {
#define CASEEQ( BUF, STR ) } else if(!strcmp(BUF, STR)) {
#define DEFAULT } else {
#define ENDSWITCH }

void read_config_from_file( char* file_name )
{
	FILE* inf;
	int x, y, val;
	inf = fopen_s( file_name, "r" );
	char buf[BUF_SIZE];
	fgets( buf, BUF_SIZE, inf );
	if( !strcmp( buf, "Map" )) {
		errx( 20, "Map expected in config file" );
	}
	fscanf( inf, "%dx%d", &map.w, map.h );
	/* Include borders in our map */
	map.w += 2;
	map.h += 2;

	map.m = (char*) malloc_s( map.h * sizeof( char* ));
	for( y = 0; y < map.h; ++y ) {
		map.m[y] = (char*) malloc_s( map.w * sizeof( char ));
		fgets( map.m[y], map.w, inf );
	}

	while( fgets( buf, BUF_SIZE, inf ) != NULL) {
		if( !strcmp( buf, "items:" )) {
			break;
		}
		fscanf( inf, "%d", &val );
		SWITCH
		CASEEQ( buf, "initial_health" )
			game.initial_health = val;
		CASEEQ( buf, "hit_value" )
			game.hit_value = val;
		CASEEQ( buf, "recharge_duration" )
			game.recharge_duration = val;
		CASEEQ( buf, "mining_time" )
			game.mining_time = val;
		CASEEQ( buf, "stay_health_drop" )
			game.stay_health_drop = val;
		CASEEQ( buf, "movement_health_drop" )
			game.movement_health_drop = val;
		CASEEQ( buf, "step_standard_delay" )
			game.step_standard_delay = val;
		CASEEQ( buf, "moratory_duration" )
			game.moratory_duration = val;
		DEFAULT
			errx( 21, "Unknown game parameter" );
		ENDSWITCH
	}

	while( fscanf( inf, "%d %d %d", &x, &y, &val )) {
		map.m[y][x] = BOOST;
	};

}