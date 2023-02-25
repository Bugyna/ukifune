#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <wchar.h>

#include "core.h"

typedef struct
{
	ENTITY e;
	int hp, mana;
} PLAYER;


typedef struct
{
	WIN win;
	WORLD world;
	PLAYER p;
	int state;
} GAME;


GAME game;

void update(GAME* game, PLAYER* p)
{
	win_event_handle(&game->win);
}

int main(int argc, char* argv[])
{
	// game_init(&game);

	PLAYER player;
	win_init(&game.win);
	world_init(&game.world);
	player.e = entity_create_from_texture(200, 200, create_texture_from_image(&game.win, 200, 200, 200, 200, "maybe.png"));
	ENTITY bg = entity_create_from_texture(200, 200, create_texture_from_image(&game.win, 200, 200, 400, 400, "test.jpg"));
	map_add_entity(&game.world.maps[0], bg);
	map_add_entity(&game.world.maps[0], player.e);
	win_set_render_queue(&game.win, &game.world.maps[0], 0, 0, 0, 0);

	widget_bind(&game.win.children[0], "<Escape>", win_close);
	// CAMERA camera;
	// game.p.entity = entity_create_from_texture(...);

	while (game.win.is_running)
	{
		// win.render(win, world);
		update(&game, &player);
		win_render_default(&game.win);
	}

	return 0;
}


