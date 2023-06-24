#pragma once
#include "components.h"
#include "widgets.h"


void change_texture_to_text(WIN* win, TEXTURE* t, const char* text, SDL_Color color)
{
	if (text == NULL) text = "!!NULL";
	SDL_FreeSurface(t->surf);
	t->surf = TTF_RenderUTF8_Blended(global_font, text, color);
	SDL_DestroyTexture(t->tex);
	t->tex = SDL_CreateTextureFromSurface(win->renderer, t->surf);
	t->rect = (SDL_Rect){t->rect.x, t->rect.y, t->surf->w, t->surf->h};
}


void change_texture_to_int(WIN* win, TEXTURE* t, int num, SDL_Color color)
{
	char* text = malloc(20);
	sprintf(text, "%d", num);

	change_texture_to_text(win, t, text, color);
	free(text);
}


TEXTURE create_texture_from_text(WIN* win, int x, int y, const char* text, SDL_Color color)
{
	TEXTURE tex;
	tex.surf = TTF_RenderUTF8_Blended(global_font, text, color);
	UKI_ASSERT(tex.surf, "something went wrong when creating texture from text: '%s'", text);
	tex.tex = SDL_CreateTextureFromSurface(win->renderer, tex.surf);
	// tex.rect = (SDL_Rect){x, y, tex.surf->w, tex.surf->h};
	tex.rect = (SDL_Rect){x, y, tex.surf->w, tex.surf->h};

	return tex;
}


TEXTURE create_texture_from_int(WIN* win, int x, int y, int num, SDL_Color color)
{
	TEXTURE tex;
	char* text = malloc(20);
	sprintf(text, "%d", num);

	create_texture_from_text(win, x, y, text, color);
	free(text);

	return tex;
}


TEXTURE create_texture_from_image(WIN* win, int x, int y, int w, int h, const char* path)
{
	TEXTURE tex;
	// SDL_Surface* tmp = IMG_Load(path);
	// SDL_BlitSurface(tmp, NULL, tex.surf, &tex.rect);
	tex.surf = IMG_Load(path);
	// Uint32 colorkey = SDL_MapRGB(loadedSurface->format, 0, 0, 0);
	// SDL_SetColorKey(loadedSurface, SDL_TRUE, colorkey);

	UKI_ASSERT(tex.surf, "something went wrong when loading image from path: %s", path);
	tex.tex = SDL_CreateTextureFromSurface(win->renderer, tex.surf);
	// SDL_SetTextureAlphaMod(tex.tex, 0);
	if (w == -1 || h == -1)
		tex.rect = (SDL_Rect){x, y, tex.surf->w, tex.surf->h};
	else
		tex.rect = (SDL_Rect){x, y, w, h};

	return tex;
}


void rescale_texture(TEXTURE* tex, int w, int h)
{
	
	tex->rect.w = w;
	tex->rect.h = h;
}

void update_texture(TEXTURE* tex)
{
	
}

void render_texture(WIN* win, TEXTURE* tex)
{
	SDL_SetTextureBlendMode(win->renderer, SDL_BLENDMODE_NONE);
	SDL_RenderCopyEx(win->renderer, tex->tex, NULL, &tex->rect, 0, NULL, 0);
}


void render_primitive_list(WIN* w, PRIMITIVE_LIST primitive_list)
{
	ITERATE_LIST(PRIMITIVE, primitive_list, n)
	// for (PRIMITIVE_NODE* n = primitive_list.first; (n != NULL && n != n->next); n = n->next)
	{
		SDL_SetRenderDrawColor(w->renderer, n->val->color.r, n->val->color.g, n->val->color.b, 55);
		switch (n->val->type)
		{
			case P_RECT:
				SDL_RenderDrawRect(w->renderer, &n->val->r);
				SDL_SetRenderDrawColor(w->renderer, n->val->color.r, n->val->color.g, n->val->color.b, 22);
				SDL_RenderFillRect(w->renderer, &n->val->r);
			break;
			case P_POINT:
				SDL_SetRenderDrawColor(w->renderer, n->val->color.r, n->val->color.g, n->val->color.b, 22);
				SDL_RenderDrawPoint(w->renderer, n->val->p.x, n->val->p.y);
			break;
			case P_CIRCLE:
				SDL_SetRenderDrawColor(w->renderer, n->val->color.r, n->val->color.g, n->val->color.b, 255);
				uki_draw_circle(w->renderer, n->val->c);
			break;
		}
	}
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


