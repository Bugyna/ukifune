#pragma once
#include "components.h"
#include "widgets.h"


TEXTURE create_texture_from_text(WIN* win, int x, int y, char* text, SDL_Color color)
{
	TEXTURE tex;
	tex.surf = TTF_RenderUTF8_Blended(global_font, text, color);
	tex.tex = SDL_CreateTextureFromSurface(win->renderer, tex.surf);
	tex.rect = (SDL_Rect){x, y, tex.surf->w, tex.surf->h};

	return tex;
}

TEXTURE create_texture_from_image(WIN* win, int x, int y, int w, int h, const char* path)
{
	TEXTURE tex;
	// SDL_Surface* tmp = IMG_Load(path);
	// SDL_BlitSurface(tmp, NULL, tex.surf, &tex.rect);
	tex.surf = IMG_Load(path);
	tex.tex = SDL_CreateTextureFromSurface(win->renderer, tex.surf);
	if (w == -1 || h == -1)
		tex.rect = (SDL_Rect){x, y, tex.surf->w, tex.surf->h};
	else
		tex.rect = (SDL_Rect){x, y, w, h};

	return tex;
}


void render_texture(WIN* win, TEXTURE* tex)
{
	SDL_RenderCopyEx(win->renderer, tex->tex, NULL, &tex->rect, 0, NULL, 0);
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


