#pragma once
// #include "components.c"

// #include "core.h"
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "util.h"
#include "world.h"

enum
{
	W_WIN,
	W_LABEL,
	W_BUTTON,
	W_TEXT_INPUT,
};


// enum
// {
	// EV_MOUSE_MOVE=1,
	// EV_MOUSE_CLICK,
	// EV_KEYPRESS,
	// EV_KEYDOWN,
	// EV_KEYPUP,
// };

#define world_set_render_fn(w, type, fn) w.render_fn = type fn

#define GET_ATTR(w, attr) w->attr
#define GET_ATTR_PTR(w, attr) &w->attr
#define DEF_GET_WIDGET_ATTR_PTR_FN(ret_type, name, attr) ret_type name(WIDGET* w) \
{\
	switch (w->type)\
	{\
		case W_LABEL:\
			return &w->label attr;\
		break;\
		case W_TEXT_INPUT:\
			return &w->text_input attr;\
		break;\
	}\
	return NULL;\
}

#define DEF_GET_WIDGET_ATTR_FN(ret_type, name, attr, DEFAULT_VALUE) ret_type name(WIDGET* w) \
{\
	switch (w->type)\
	{\
		case W_LABEL:\
			return w->label attr;\
		break;\
		case W_TEXT_INPUT:\
			return w->text_input attr;\
		break;\
	}\
	return DEFAULT_VALUE;\
}

#define EVENT SDL_Event


SDL_Color* global_colors;
// typedef struct EVENT EVENT;

typedef struct BIND BIND;
typedef struct STYLE STYLE;
typedef struct BINDS_HASHMAP BINDS_HASHMAP;
typedef struct WIN WIN;

typedef struct LABEL LABEL;
typedef struct BUTTON BUTTON;
typedef struct TEXT_INPUT TEXT_INPUT;
typedef struct WIDGET WIDGET;

struct BIND
{
	u8(*system)(WIDGET*, EVENT);
	u8(*custom)(WIDGET*, EVENT);
};

void alloc_win_hashmap(BINDS_HASHMAP* h);
struct BINDS_HASHMAP
{
	uint32_t size;
	uint32_t index;
	// void(**fn)(WIN*, EVENT);
	BIND* binds;
};

struct STYLE
{
	SDL_Color bg;
	SDL_Color fg;
	TTF_Font* font;
	// border
	// background picture
	// etc.
};

struct LABEL
{
	TEXTURE tex;
	STRING text;
};

struct BUTTON
{
	TEXTURE tex;
	STRING text;
};

struct TEXT_INPUT
{
	TEXTURE tex;
	TEXTURE cursor;
	STRING text;
};


struct WIDGET
{
	WIN* win_parent;
	WIDGET* parent;
	WIDGET* children;
	STYLE style;

	int child_index;
	void(*render_fn)(WIDGET*);


	int type;
	char* name;
	union
	{
		LABEL label;
		BUTTON button;
		TEXT_INPUT text_input;
	};
	BINDS_HASHMAP binds;

};

struct WIN
{
	SDL_Window* win;
	SDL_Renderer* renderer;

	SDL_Event* event;

	WIDGET* children;
	WIDGET* focus;
	WIDGET* attention;

	WIDGET** render_list;
	STYLE style;

	BINDS_HASHMAP binds;

	int child_index, render_index;

	int mouse_x, mouse_y;
	int mouse_scroll;
};

