#pragma once
#include "util.h"


typedef struct
{
	int x, y;
	int r, d;
} UKI_CIRCLE;


UKI_CIRCLE uki_create_circle(int x, int y, int r)
{
	// UKI_CIRCLE ret = {.x = x, .y = y, .r = r};
	// return ret;
	return (UKI_CIRCLE){.x = x, .y = y, .r = r, .d = r*2};
}


void uki_draw_circle(SDL_Renderer* renderer, UKI_CIRCLE c)
{
	int x = c.r-1, y = 0;
	int tx = 1, ty = 1;
	int error = 1 - c.d;

	while (x >= y)
	{
		SDL_RenderDrawPoint(renderer, c.x+x, c.y+y);
		SDL_RenderDrawPoint(renderer, c.x-x, c.y+y);
		SDL_RenderDrawPoint(renderer, c.x-x, c.y-y);
		SDL_RenderDrawPoint(renderer, c.x+x, c.y-y);
		SDL_RenderDrawPoint(renderer, c.x+y, c.y-x);
		SDL_RenderDrawPoint(renderer, c.x+y, c.y+x);
		SDL_RenderDrawPoint(renderer, c.x-y, c.y+x);
		SDL_RenderDrawPoint(renderer, c.x-y, c.y-x);
		// y++; x--;
		
		if (error <= 0)
      {
         ++y;
         ty += 2;
         error += ty;
      }

      if (error > 0)
      {
         --x;
         tx += 2;
         error += (tx - c.d);
      }
	}
}