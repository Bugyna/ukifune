#pragma once

#include "components.h"
#include "widgets.h"



bool collide_rect(SDL_Rect a, SDL_Rect b)
{
	if (a.x >= b.x || a.x+a.w <= b.x+b.w)
		if (a.y >= b.y || a.y+a.h <= b.y+b.h)
			return 1;

	return 0;
}

bool collide_point(int x, int y, SDL_Rect b)
{
	if (x >= b.x && x <= b.x+b.w)
		if (y >= b.y && y <= b.y+b.h)
			return 1;

	return 0;
}