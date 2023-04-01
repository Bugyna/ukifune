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
	PLAYER player;
	int state;
} GAME;


GAME game;

void update(GAME* game, PLAYER* p)
{
	win_event_handle(&game->win);
}

u8 player_move_left BIND_FN_PARAMS
{
	game.player.e.tex.rect.x -= 10;
	game.player.e.x -= 10;
	return 0;
}

u8 player_move_right BIND_FN_PARAMS
{
	game.player.e.tex.rect.x += 1 * DELTA_TIME;
	game.player.e.x += 1 * DELTA_TIME;
	return 0;
}

int main(int argc, char* argv[])
{
	// game_init(&game);

	PLAYER player;
	win_init(&game.win);
	world_init(&game.world);
	game.player.e = entity_create_from_texture(200, 200, create_texture_from_image(&game.win, 200, 200, 50, 50, "maybe.png"));
	ENTITY bg = entity_create_from_texture(200, 200, create_texture_from_image(&game.win, 200, 200, 400, 400, "test.jpg"));
	map_add_entity(&game.world.maps[0], &bg);
	map_add_entity(&game.world.maps[0], &game.player.e);
	win_set_render_queue(&game.win, &game.world.maps[0], 0, 0, 0, 0);

	widget_bind(&game.win.children[0], "<Escape>", win_close);
	widget_bind(&game.win.children[0], "a", player_move_left);
	widget_bind(&game.win.children[0], "d", player_move_right);
	// CAMERA camera;
	// game.p.entity = entity_create_from_texture(...);

	while (game.win.is_running)
	{
		// while (SDL_PollEvent(&game.win.event)) {
			// switch (game.win.event.type) {
				// case SDL_QUIT:
					// game.win.is_running = false;
				// break;
	
				// case SDL_WINDOWEVENT:
					// win_handle_window_event(&game.win, game.win.event);
				// break;
				
				// case SDL_KEYDOWN:
					// if (game.win.event.key.keysym.sym == 'a') player_move_left(&game.win.children[0], game.win.event, "a");
					// else if (game.win.event.key.keysym.sym == 'd') player_move_right(&game.win.children[0], game.win.event, "d");
				// break;
			// }
		// }
		update(&game, &player);
		win_render_default(&game.win);
		cap_fps();
		// cap_fps_fixed(25);
	}

	return 0;
}


