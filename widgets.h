#pragma once
#include "util.h"


enum
{
	W_WORLD,
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

#define GET_ATTR(w, attr) w->attr
#define GET_ATTR_PTR(w, attr) &w->attr
#define DEF_FN(name, ret_type, attr) ret_type name(WIDGET* w) \
{\
	switch (w->type)\
	{\
		case W_LABEL:\
			return w->label.attr;\
		break;\
	}\
}

#define EVENT SDL_Event


SDL_Color* global_colors;
// typedef struct EVENT EVENT;

typedef struct BIND BIND;
typedef struct STYLE STYLE;
typedef struct BINDS_HASHMAP BINDS_HASHMAP;
typedef struct WORLD WORLD;

typedef struct LABEL LABEL;
typedef struct BUTTON BUTTON;
typedef struct TEXT_INPUT TEXT_INPUT;
typedef struct WIDGET WIDGET;


struct BIND
{
	u8(*system)(WORLD*, EVENT);
	u8(*custom)(WORLD*, EVENT);
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
	char* text;
};

struct BUTTON
{
	TEXTURE tex;
	char* text;
};

struct TEXT_INPUT
{
	TEXTURE tex;
	char* text;
	int len;
	int index;
};


struct WIDGET
{
	WORLD* world_parent;
	WIDGET* parent;
	WIDGET* children;
	STYLE style;

	int child_index;

	int type;
	char* name;
	union
	{
		LABEL label;
		BUTTON button;
		TEXT_INPUT input;
	};
};


void alloc_world_hashmap(BINDS_HASHMAP* h);
struct BINDS_HASHMAP
{
	uint32_t size;
	uint32_t index;
	// void(**fn)(WORLD*, EVENT);
	BIND* binds;
};

struct WORLD
{
	WIDGET* children;
	WIDGET* focus;
	WIDGET* attention;
	STYLE style;

	BINDS_HASHMAP keydown;
	BINDS_HASHMAP keyup;
	BINDS_HASHMAP mouse;

	int child_index;

	int mouse_x, mouse_y;
	int mouse_scroll;
};

// bool world_check_collision(WORLD* w, 

char* widget_type_name(WIDGET w)
{
	switch (w.type)
	{
		case W_LABEL:
			return "LABEL";
		break;
	}

	return "UNDEFINED";
}


char* generate_widget_name(WORLD* w, WIDGET widget)
{
	char* name = malloc(32);
	strcpy(name, widget_type_name(widget));
	char* num = malloc(12);
	sprintf(num, "%d", w->child_index);
	strcat(name, num);

	return name;
}


WIDGET create_label(SDL_Renderer* renderer, WORLD* w, int x, int y, char* text)
{
	WIDGET widget;
	widget.style = w->style;
	LABEL l;

	l.tex = create_texture_from_text(renderer, x, y, text, widget.style.fg);

	widget.type = W_LABEL;
	widget.world_parent = w;
	widget.label = l;
	widget.name = generate_widget_name(w, widget);
	return widget;
}


void widget_handle_keydown(WIDGET* w, EVENT e)
{
	
}

void widget_bind(WIDGET* w, void(*fn)(WORLD*, EVENT))
{
	
}


SDL_Rect* get_widget_rect_ptr(WIDGET* w)
{
	switch (w->type)
	{
		case W_LABEL:
			return &w->label.tex.rect;
		break;
	}

	return &OUT_OF_BOUNDS_RECT;
}


SDL_Rect get_widget_rect(WIDGET* w)
{
	return *get_widget_rect_ptr(w);
}


TEXTURE* get_widget_texture_ptr(WIDGET* w)
{
	switch (w->type)
	{
		case W_LABEL:
			return &w->label.tex;
		break;
	}

	return NULL;
}

void widget_handle_attention_get(WIDGET* w)
{
	
}


void widget_handle_attention_release(WIDGET* w)
{
	
}


void widget_handle_focus_get(WIDGET* w)
{
	
}


void widget_handle_focus_release(WIDGET* w)
{
	
}