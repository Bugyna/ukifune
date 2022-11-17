#pragma once
#include "core.h"
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#define WHITE (SDL_Color){255, 255, 255}
TTF_Font* font;
#define OUT_OF_BOUNDS_RECT (SDL_Rect){-1, -1, -1, -1}

enum
{
	C_COLLIDER,
	C_RIGIDBODY,
	C_TEXTURE,
};

typedef struct COLLIDER COLLIDER;
typedef struct RIGIDBODY RIGIDBODY;
typedef struct TEXTURE TEXTURE;
typedef struct STRING STRING;

struct STRING
{
	char* str;
	int index, size;
};

struct COLLIDER
{
	int type;
	SDL_Rect* rect;
};


struct RIGIDBODY
{
	int type;
	COLLIDER coll;
	
};

struct TEXTURE
{
	int type;
	SDL_Rect rect;
	SDL_Texture* tex;
	SDL_Surface* surf;
};



TEXTURE create_texture_from_text(SDL_Renderer* renderer, int x, int y, char* text, SDL_Color color)
{
	TEXTURE tex;
	tex.surf = TTF_RenderUTF8_Blended(font, text, color);
	tex.tex = SDL_CreateTextureFromSurface(renderer, tex.surf);
	tex.rect = (SDL_Rect){x, y, tex.surf->w, tex.surf->h};
	tex.type = C_TEXTURE;

	return tex;
}

TEXTURE create_texture_from_image(SDL_Renderer* renderer, int x, int y, int w, int h, const char* path)
{
	TEXTURE tex;
	// SDL_Surface* tmp = IMG_Load(path);
	// SDL_BlitSurface(tmp, NULL, tex.surf, &tex.rect);
	tex.surf = IMG_Load(path);
	tex.tex = SDL_CreateTextureFromSurface(renderer, tex.surf);
	if (w == -1 || h == -1)
		tex.rect = (SDL_Rect){x, y, tex.surf->w, tex.surf->h};
	else
		tex.rect = (SDL_Rect){x, y, w, h};
	tex.type = C_TEXTURE;

	return tex;
}


// void update_texture_from_text(SDL_Renderer* renderer, int x, int y, char* text, SDL_Color color)
// {
	// TEXTURE tex;
	// tex.surf = TTF_RenderUTF8_Blended(font, text, color);
	// tex.tex = SDL_CreateTextureFromSurface(renderer, tex.surf);
	// tex.rect = (SDL_Rect){x, y, tex.surf->w, tex.surf->h};
	// tex.type = C_TEXTURE;

	// return tex;
// }


