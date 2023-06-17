#pragma once

#include "util.h"
#include "components.c"
#include "entity.h"



TEXTURE* slice_map(WIN* win, char* path, int rows, int columns, int offset_x, int offset_y, int w, int h)
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
			SDL_Surface* tmp = SDL_CreateRGBSurface(0, 50, 50, 32, 0, 0, 0, 0);

			tmpr = r;
			tmpr.x += 100;
			tmpr.y += 100;
			tmpr.w = 100;
			tmpr.h = 100;
			SDL_BlitSurface(surf,
                    &tmpr,
                    tmp,
                    &dstr);
			
			tex[i].surf = tmp;
			// tex[i].rect = dstr;
			tex[i].tex = SDL_CreateTextureFromSurface(win->renderer, tmp);
			SDL_Log("i: %d %d %d %d %d", i, r.w, r.h, tmp->w, tmp->h);
			tex[i].rect = dstr;
			// SDL_RenderCopyEx(win->renderer, tex[i].tex, NULL, &tex[i].rect, 0, NULL, 0);
			r.x += offset_x;
			i++;
		}
		r.x = 0;
		r.y += offset_y;
	}
	// tex.tex = SDL_CreateTextureFromSurface(win->renderer, tex.surf);
	// if (w == -1 || h == -1)
		// tex.rect = (SDL_Rect){0, 0, tex.surf->w, tex.surf->h};

	return tex;
}