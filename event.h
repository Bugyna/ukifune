#pragma once
#include <SDL2/SDL_event.h>




typedef struct UKI_EVENT
{
	int type;
	SDL_Event e;
};

int uki_poll_event(WIN* win)
{
	int ret = SDL_PollEvent(win->event);
	// TODO: custom event struct thingy

	return ret;
}