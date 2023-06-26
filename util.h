#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "error.h"
#include "hashmap.h"
#include "linked_list.h"

#define i8 int8_t
#define i16 int16_t
#define i32 int32_t
#define i64 int64_t
#define i128 int128_t

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t
#define u64 uint64_t
#define u128 uint128_t




#define DEBUGGING 1


static const SDL_Color COLOR_WHITE = {255, 255, 255};


bool debugging = true;
TTF_Font* global_font;

typedef struct COLORS COLORS;
typedef struct RGB RGB;
typedef struct ARGB ARGB;


struct COLORS
{
	u32 size;
	u32 index;
	SDL_Color* colors;
};

struct ARGB
{
	u8 a, r, g, b;
};

struct RGB
{
	u8 r, g, b;
};

SDL_Color create_color_from_hex(int hex)
{
	return (SDL_Color){(hex & 0xFF0000) >> 16, (hex & 0x00FF00) >> 8, (hex & 0x0000FF)};
}

RGB convert_hex_to_rgb(int hex)
{
	RGB rgb;
	rgb.r = (hex & 0xFF0000) >> 16;
	rgb.g = (hex & 0x00FF00) >> 8;
	rgb.b = (hex & 0x0000FF);
	return rgb;
}

ARGB convert_hex_to_argb(int hex)
{
	ARGB argb;
	argb.r = (hex & 0xFF000000) >> 24;
	argb.g = (hex & 0x00FF0000) >> 16;
	argb.b = (hex & 0x0000FF00) >> 8;
	argb.a = (hex & 0x000000FF);
	return argb;
}


int hash(const char* key)
{
	UKI_ASSERT(key != NULL, "trying to hash an empty key %s", "aha");

	int mask = 1 << strlen(key);
	for (unsigned int i = 0; i < strlen(key); i++) {
		mask += (int)key[i];
	}
	return mask ^ (mask >> 4);
}


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