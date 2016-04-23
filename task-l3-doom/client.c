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
	read_config_from_file("config.cfg");
	set_canonical();
	while(get_input()) {
		render();
	}
	restore();
	return 0;
}