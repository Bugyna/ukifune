#pragma once

#include "util.h"
#include "components.c"
#include "entity.h"



// ATLAS* slice_map(WIN* win, char* path, int rows, int columns, int offset_x, int offset_y, int w, int h)
// {
	
	// ATLAS* atlas = malloc(sizeof(ATLAS));
	
	// UKI_ASSERT(surf, "something went wrong when loading image from path: %s", path);
	// int i = 0;

	// atlas->rect = {0, 0, w, h};
	// atlas->rows = rows;
	// atlas->columns = columns;
	// atlas->offset_x = offset_x;
	// atlas->offset_y = offset_y;

	// atlas->surf = IMG_Load(path);
	// atlas->tex = SDL_CreateTextureFromSurface(win->renderer, tmp);

	// return tex;
// }

TEXTURE* slice_map(WIN* win, char* path, int rows, int columns, int move_x, int move_y, int offset_x, int offset_y, int w, int h, float scale)
{
	
	TEXTURE* tex = malloc(rows*columns*sizeof(TEXTURE));
	// TEXTURE** tex;
	SDL_Surface* surf = IMG_Load(path);
	UKI_ASSERT(surf, "something went wrong when loading image from path: %s", path);
	int i = 0;

	SDL_Rect r = {0, 0, offset_x, offset_y};
	SDL_Rect tmpr = {0, 0, offset_x, offset_y};
	SDL_Rect dstr = {0, 0, w, h};
	for (int y = 0; y < rows; y++)
	{
		for (int x = 0; x < columns; x++)
		{
			dstr.x = 0;
			dstr.y = 0;
			// SDL_Surface* tmp = malloc(sizeof(SDL_Surface));
			// SDL_Surface* tmp = SDL_CreateRGBSurface(0, w, h, 32, 0xff, 0xff00, 0xff0000, 0xff000000);
			// SDL_Surface* tmp = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0x000000ff);
			SDL_Surface* tmp = SDL_CreateRGBSurfaceWithFormat(0, w, h, 32, SDL_PIXELFORMAT_RGBA8888);

			tmpr = r;
			tmpr.x += offset_x;
			tmpr.y += offset_y;
			tmpr.w = w;
			tmpr.h = h;
			SDL_BlitSurface(surf,
                    &tmpr,
                    tmp,
                    &dstr);
			
			tex[i].surf = tmp;
			// tex[i].rect = dstr;

			tex[i].tex = SDL_CreateTextureFromSurface(win->renderer, tmp);
			// SDL_SetTextureBlendMode(tex[i].tex, SDL_BLENDMODE_BLEND);
			// SDL_SetTextureAlphaMod(tex[i].tex, 0xFF);
			SDL_Log("i: %d %d %d %d %d", i, r.w, r.h, tmp->w, tmp->h);
			tex[i].rect = dstr;
			tex[i].rect.w *= scale;
			tex[i].rect.h *= scale;
			// SDL_RenderCopyEx(win->renderer, tex[i].tex, NULL, &tex[i].rect, 0, NULL, 0);
			r.x += move_x;
			i++;
		}
		r.x = 0;
		r.y += move_y;
	}
	// tex.tex = SDL_CreateTextureFromSurface(win->renderer, tex.surf);
	// if (w == -1 || h == -1)
		// tex.rect = (SDL_Rect){0, 0, tex.surf->w, tex.surf->h};

	return tex;
}