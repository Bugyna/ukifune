#pragma once
#include "components.c"
#include "entity.h"

typedef struct
{
	TILE* tiles;
	int tile_size, tile_index;
	
	int entity_index, render_list_index;
	int entity_size, render_list_size;

	
	ENTITY** entity_list;
	ENTITY** render_list;
} MAP;


void map_init(MAP* map)
{
	map->render_list_index = 0;
	map->render_list_size = 20;
	map->render_list = malloc(map->render_list_size*sizeof(ENTITY*));

	map->entity_index = 0;
	map->entity_size = 20;
	map->entity_list = malloc(map->entity_size*sizeof(ENTITY));

	map->tile_index = 0;
	map->tile_size = 100;
	map->tiles = malloc(map->tile_size*sizeof(TILE));
}

void map_add_entity(MAP* map, ENTITY* e)
{
	map->entity_list[map->entity_index++] = e;
}

void map_add_renderable_entity(MAP* map, ENTITY* e)
{
	map_add_entity(map, e);
	map->render_list[map->render_list_index++] = map->entity_list[map->entity_index-1];
}


