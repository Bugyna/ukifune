#include <stdlib.h>
#include <stdio.h>
#include "core.h"

struct PLAYER
{
	ENTITY e;
	int hp, mana;
};


struct GAME
{
	WORLD* scenes;
	WIN win;
	PLAYER p;
	int state;
};


GAME game;

void render(WIN* win, WORLD* w)
{
	// win->renderer...()
	for (int i = 0; i < w->entity_index; i++)
	{
		// win->renderer.Blit...()
	}

	// win->renderer...()
}


void update(WIN* win, WORLD* w, PLAYER* p)
{
	while (uki_poll_event(win))
	{
		
	}
}

int main(int argc, char* argv[])
{
	game_init(&game);
	win_init(&game.win);
	world_init(&game.world[0]);

	CAMERA camera;
	game.p.entity = entity_create_from_texture(...);

	while (world->state != STATE_EXIT)
	{

		// win.render(win, world);
		update(&world, &player);
		render(&win, &world);
	}

	return 0;
}


