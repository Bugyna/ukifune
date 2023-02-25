#pragma once
#include "linked_list.h"
#include "entity.h"

void render_entity(WIN* win, ENTITY* e)
{
	render_texture(win, &e->tex);
}