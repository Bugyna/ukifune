#pragma once
#include "core.h"
#include "map.h"


enum
{
	STATE_RUN,
	STATE_PAUSE,
	STATE_MENU,
	STATE_CUTSCENE,
};


typedef struct WORLD WORLD;


struct WORLD
{
	MAP* maps;
	int map_size, map_index, cur_map;
	int state;
	
};


void world_init(WORLD* w)
{
	w->state = STATE_RUN;
	w->cur_map = 0;
	w->map_index = 0;
	w->map_size = 5;
	w->maps = malloc(w->map_size*sizeof(MAP));

	for (int i = 0; i < w->map_size; i++) {
		map_init(&w->maps[i]);
	}
}

// void world_add_entity(WORLD* w, ENTITY e)
// {
	// w->entities[w->entity_index++] = e;
// }

// void world_add_renderable_entity(WORLD* w, ENTITY e)
// {
	// world_add_entity(w, e);
	// w->render_list[w->render_list_index++] = &w->entities[w->entity_index-1];
// }

