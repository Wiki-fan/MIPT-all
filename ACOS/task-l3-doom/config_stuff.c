#include "config_stuff.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <err.h>
#include "../common/utils.h"
#include "game_stuff.h"
#include "net_stuff.h"
#include "common_types.h"

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
mining,
game.mining = (int)val;
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
int get_raw( FILE* f, char* buf, int n )
{
    int c;
    int i = 0;
    while(( c = getc( f )) != EOF)
    {
        if( c == '\n' )
        {
            break;
        }
        buf[i++] = (char) c;
        if( i == n + 1 )
        {
            break;
        }
    }
    return i;
}

/** Read config from file */
void read_config_from_file( char* file_name )
{
    FILE* inf;
    int x, y;
    float val;
    char buf[BUF_SIZE];
    LOG(( "Starting to read config file" ));
    CNULL(inf = fopen_s( file_name, "r" ), E_FOPEN);
    fscanf( inf, "%10s", buf );
    if( strcmp( buf, "Map" ))
    {
        errx( 20, "Map expected in config file" );
    }
    fscanf( inf, "%dx%d\n", &game.map.h, &game.map.w );
    /* Include borders in our map */
    game.map.w += 2;
    game.map.h += 2;

    game.map.fg = (char**) malloc_s( game.map.h * sizeof( char* ));
    game.map.bg = (int**) malloc_s( game.map.h * sizeof( int* ));
    game.map.pl = NULL;
    game.map.splash = NULL;
    for( y = 0; y < game.map.h; ++y )
    {
        game.map.fg[y] = (char*) malloc_s(( game.map.w ) * sizeof( char ));
        game.map.bg[y] = (int*) calloc_s( game.map.w, sizeof( int ));
        get_raw( inf, game.map.fg[y], game.map.w );
    }

    while( fscanf( inf, "%100s", buf ) != 0 )
    {
        LOG(( "Reading %s", buf ));
        if( !strcmp( buf, "items:" ))
        {
            break;
        }
        fscanf( inf, " = %f\n", &val );
        SWITCH
        CASEEQ( buf, "initial_health" )
        game.initial_health = (int) val;
        CASEEQ( buf, "hit_value" )
        game.hit_value = (int) val;
        CASEEQ( buf, "recharge_duration" )
        game.recharge_duration = (int) val;
        CASEEQ( buf, "mining" )
        game.mining_time = (int) val;
        CASEEQ( buf, "stay_health_drop" )
        game.stay_health_drop = (int) val;
        CASEEQ( buf, "movement_health_drop" )
        game.movement_health_drop = (int) val;
        CASEEQ( buf, "step_standard_delay" )
        game.step_standard_delay = val;
        CASEEQ( buf, "moratory_duration" )
        game.moratory_duration = (int) val;
        DEFAULT
        errx( 21, "Unknown game parameter %s", buf );
        ENDSWITCH
    }
    game.fps = (int)(1.0/game.step_standard_delay);
    /*game.recharge_duration *= fps;
    game.mining_time *= fps;
    game.moratory_duration *= fps;*/

    while( fscanf( inf, "%d %d %f", &x, &y, &val ) != EOF)
    {
        game.map.fg[y][x] = M_BONUS;
        game.map.bg[y][x] = (int) val;
    }

    LOG(( "Config read successfully" ));

}