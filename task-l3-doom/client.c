#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <err.h>
#include "../common/utils.h"
#include "game_stuff.h"
#include "tty_stuff.h"
#include "config_stuff.h"

int main()
{
	read_config_from_file( "config.cfg" );
	set_canonical();
	player_init();
	do {
		render();
	} while( get_input());

	restore();
	return 0;
}