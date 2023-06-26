#pragma once
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "util.h"
#include "circle.h"
#include "animator.h"

#define WHITE (SDL_Color){255, 255, 255}
TTF_Font* font;
#define OUT_OF_BOUNDS_RECT (SDL_Rect){-1, -1, -1, -1}

enum
{
	C_EMPTY=0,
	C_COLLIDER,
	C_RIGIDBODY,
	C_TEXTURE,
	C_ATLAS,
	C_ANIMATOR,
	C_GRAVITY,
};

typedef enum
{
	P_RECT=1,
	P_POINT,
	P_CIRCLE,
} PRIMITIVE_TYPE;


typedef struct ENTITY ENTITY;

typedef struct COMPONENT COMPONENT;
typedef struct COLLIDER COLLIDER;
typedef struct RIGIDBODY RIGIDBODY;
typedef struct TEXTURE TEXTURE;
typedef struct STRING STRING;

typedef struct
{
	PRIMITIVE_TYPE type;
	SDL_Color color;
	union
	{
		SDL_Rect r;
		SDL_Point p;
		UKI_CIRCLE c;
		
	};
} PRIMITIVE;
DEFINE_LINKED_LIST(PRIMITIVE, PRIMITIVE)


struct STRING
{
	char* str;
	int index, size;
};

struct COLLIDER
{
	ENTITY* e;
	SDL_Rect r;
};


struct RIGIDBODY
{
	COLLIDER coll;
};


typedef struct
{
	ENTITY* e;
	float force;

} GRAVITY;

struct TEXTURE
{
	SDL_Rect rect;
	SDL_Texture* tex;
	SDL_Surface* surf;
	double angle;
};


typedef struct
{
	SDL_Rect rect;
	int rows, columns;
	int offset_x, offset_y;
	int w, h;

	SDL_Texture* tex;
	SDL_Surface* surf;
} ATLAS;


struct COMPONENT
{
	int type;
	union
	{
		TEXTURE* tex;
		ATLAS* atlas;
		ANIMATOR* anim;
		GRAVITY* g;
		COLLIDER* coll;
	};
};

DEFINE_LINKED_LIST(COMPONENT, COMPONENT)
DEFINE_HASHMAP(COMPONENT_MAP, COMPONENT)

typedef struct
{
	int a, b;
} VEC2;

typedef struct
{
	int a, b, c;
} VEC3;

typedef struct
{
	int a, b, c, d;
} VEC4;

typedef struct
{
	int a, b, c, d;
} RECT;




