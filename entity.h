#pragma once
#include "components.h"

#define DEF_GET_COMPONENT_ATTR_PTR_FN(ret_type, name, attr) ret_type name(COMPONENT* c) \
{\
	switch (c->type)\
	{\
		case C_TEXTURE:\
			return &c-> attr;\
		break;\
	}\
	return NULL;\
}

#define DEF_GET_COMPONENT_ATTR_FN(ret_type, name, attr, DEFAULT_VALUE) ret_type name(WIDGET* c) \
{\
	switch (c->type)\
	{\
		case C_TEXTURE:\
			return &c-> attr;\
		break;\
	}\
	return DEFAULT_VALUE;\
}

enum
{
	ENTITY_COMPONENT_LIST,
	ENTITY_TILE,
};

typedef struct ENTITY ENTITY;


typedef struct
{
	int component_index, component_size;
	COMPONENT* components;
} COMPONENT_LIST;

typedef struct
{
	SDL_Rect pos, abs_pos;
	TEXTURE tex;
} TILE;

struct ENTITY
{
	char* name;
	int x, y;
	SDL_Rect view;
	// SDL_Rect pos, abs_pos;
	SDL_Rect* colliders;
	int collider_index, collider_size;
	TEXTURE tex;
};

DEFINE_LINKED_LIST(ENTITY, ENTITY, e)

// DEF_GET_COMPONENT_ATTR_PTR_FN(TEXTURE*, get_component_texture_ptr, tex)

// void entity_init(ENTITY* e)
// {
	// e->component_index = 0;
	// e->component_size = 3;
	// e->components = malloc(e->component_size*sizeof(COMPONENT));
// }


ENTITY entity_create_from_texture(int x, int y, TEXTURE tex)
{
	ENTITY e;
	e.name = "dwa";
	e.x = x;
	e.y = y;
	e.collider_index = 0;
	e.collider_size = 5;
	e.colliders = malloc(e.collider_size*sizeof(SDL_Rect));
	e.tex = tex;

	return e;
}

