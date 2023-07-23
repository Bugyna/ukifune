#pragma once
#include "components.h"
#include "util.h"

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

#define ENTITY_SET(e_pos, n) *e_pos = n
#define ENTITY_SET_REL(e_pos, n) *e_pos = *e_pos + n


enum
{
	STATE_IDLE,
	STATE_MOVE,
	STATE_JUMP,
	STATE_FALL,
};

typedef struct ENTITY ENTITY;

#define ENTITY_BIND_PARAMS (ENTITY* entity, EVENT e, BIND* bind)


typedef struct
{
	SDL_Rect pos, abs_pos;
	TEXTURE tex;
} TILE;



struct ENTITY
{
	u32 id;
	char* name;
	int state;

	int abs_x, abs_y;

	SDL_Rect pos;
	TEXTURE* tex;

	// COMPONENT_LIST components;
	COMPONENT_MAP components;
};


DEFINE_LINKED_LIST(ENTITY_LIST, ENTITY)
DEFINE_HASHMAP(ENTITY_MAP, ENTITY)
ENTITY_MAP global_entities;

