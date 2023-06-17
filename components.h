#pragma once
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "util.h"
#include "circle.h"

#define WHITE (SDL_Color){255, 255, 255}
TTF_Font* font;
#define OUT_OF_BOUNDS_RECT (SDL_Rect){-1, -1, -1, -1}

enum
{
	C_COLLIDER,
	C_RIGIDBODY,
	C_TEXTURE,
};

typedef enum
{
	P_RECT=1,
	P_POINT,
	P_CIRCLE,
} PRIMITIVE_TYPE;

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
	SDL_Rect* rect;
};


struct RIGIDBODY
{
	COLLIDER coll;
};

struct TEXTURE
{
	SDL_Rect rect;
	SDL_Texture* tex;
	SDL_Surface* surf;
};


struct COMPONENT
{
	int type;
	union
	{
		TEXTURE tex;
		RIGIDBODY rb;
	};
};

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

