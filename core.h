#pragma once
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "components.c"
#include "entity.h"
#include "util.h"
#include "widgets.c"
#include "win.c"
#include "world.h"


void uki_destroy_win(WIN* win)
{
	SDL_DestroyRenderer(win->renderer);
	SDL_DestroyWindow(win->win);
    
	TTF_CloseFont(win->style.font);
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}


int uki_poll_event(WIN* win)
{
	int ret = SDL_PollEvent(win->event);
	// TODO: custom event struct thingy

	return ret;
}