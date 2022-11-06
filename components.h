#pragma once
#include <SDL2/SDL_video.h>

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