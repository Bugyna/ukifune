#pragma once
#include "world.h"
#include "win.c"



void win_set_render_queue(WIN* win, MAP* map, int x, int y, int cx, int cy)
{
	for (int i = 0; i < map->entity_index; i++)
	{
		if (map->entity_list[i]->name == NULL) break;
		ENTITY_LIST_APPEND(&win->render_list, map->entity_list[i]);
	}
}
