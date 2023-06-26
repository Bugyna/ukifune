#pragma once
#include "linked_list.h"
#include "entity.h"

void render_entity(WIN* win, ENTITY* e)
{
	e->pos.w = e->tex->rect.w;
	e->pos.h = e->tex->rect.h;
	render_texture_with_pos(win, e->tex, &e->pos);
}



// void entity_init_from_texture(ENTITY* e, TEXTURE tex)
// {
	// e->name = "dwa";
	
	// e->collider_index = 0;
	// e->collider_size = 5;
	// e->colliders = malloc(e->collider_size*sizeof(SDL_Rect));
	// e->tex = tex;

	// e->x = &e->tex.rect.x;
	// e->y = &e->tex.rect.y;
// }


ENTITY* entity_create()
{
	ENTITY* e = malloc(sizeof(ENTITY));
	e->id = 0;
	e->name = "0";
	e->state = STATE_IDLE;
	e->abs_x = 0;
	e->abs_y = 0;

	e->pos = (SDL_Rect){0,0,0,0};
	e->tex = NULL;

	COMPONENT_MAP_INIT(&e->components, 20);
	return e;
}

void entity_add_component(ENTITY* e, COMPONENT* c)
{
	COMPONENT_MAP_ADD_AT_INDEX(&e->components, c->type, c);
}

void entity_add_animator(ENTITY* e, ANIMATOR* anim)
{
	COMPONENT* c = malloc(sizeof(COMPONENT));
	c->anim = anim;
	c->type = C_ANIMATOR;
	entity_add_component(e, c);
}


void entity_move(ENTITY* e, int x, int y)
{
	e->pos.x = x;
	e->pos.y = y;
}

// ENTITY* entity_create_from_texture(TEXTURE tex)
// {
	// ENTITY* e;
	// e->name = "dwa";
	
	// e->collider_index = 0;
	// e->collider_size = 5;
	// e->colliders = malloc(e->collider_size*sizeof(SDL_Rect));
	// e->tex = tex;

	// e->x = &e->tex.rect.x;
	// e->y = &e->tex.rect.y;

	// return e;
// }


