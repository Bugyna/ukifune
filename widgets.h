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
#define DEF_GET_MEMBER_ATTR_PTR_FN(ret_type, name, attr) ret_type name(WIDGET* w) \
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

#define DEF_GET_MEMBER_ATTR_FN(ret_type, name, attr, DEFAULT_VALUE) ret_type name(WIDGET* w) \
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

void alloc_world_hashmap(BINDS_HASHMAP* h);
struct BINDS_HASHMAP
{
	uint32_t size;
	uint32_t index;
	// void(**fn)(WORLD*, EVENT);
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
		TEXT_INPUT text_input;
	};
	BINDS_HASHMAP binds;
};

struct WORLD
{
	WIDGET* children;
	WIDGET* focus;
	WIDGET* attention;
	STYLE style;

	BINDS_HASHMAP binds;

	int child_index;

	int mouse_x, mouse_y;
	int mouse_scroll;
};

// bool world_check_collision(WORLD* w, 

char* widget_type_name(WIDGET w)
{
	// return WIDGET_TYPE(w.type);
	switch (w.type)
	{
		case W_LABEL:
			return "LABEL";
		break;

		case W_TEXT_INPUT:
			return "TEXT_INPUT";
		break;
	}

	return "UNDEFINED";
}


char* generate_widget_name(WORLD* w, WIDGET* widget)
{
	char* name = malloc(32);
	strcpy(name, widget_type_name(*widget));
	char* num = malloc(12);
	sprintf(num, "%d", w->child_index);
	strcat(name, num);

	return name;
}


void widget_init(WORLD* w, WIDGET* widget, int type)
{
	widget->world_parent = w;
	widget->style = w->style;
	widget->type = type;
	widget->name = generate_widget_name(w, widget);

	widget->binds.size = 20;
	widget->binds.index = 0;
	widget->binds.binds = calloc(widget->binds.size, sizeof(BIND));

	widget->child_index = 0;
	widget->children = calloc(5, sizeof(WIDGET));
}


WIDGET create_label(SDL_Renderer* renderer, WORLD* w, int x, int y, char* text)
{
	WIDGET widget;
	widget_init(w, &widget, W_LABEL);
	LABEL l;
	l.text.size = 64;
	l.text.index = 0;
	l.text.str = malloc(l.text.size);

	l.tex = create_texture_from_text(renderer, x, y, text, widget.style.fg);

	widget.label = l;
	return widget;
}


WIDGET create_text_input(SDL_Renderer* renderer, WORLD* w, int x, int y, char* text)
{
	WIDGET widget;
	widget_init(w, &widget, W_TEXT_INPUT);
	TEXT_INPUT t;

	t.text.size = 64;
	t.text.index = 0;
	t.text.str = malloc(t.text.size);
	t.tex = create_texture_from_text(renderer, x, y, text, widget.style.fg);
	// t.cursor = create_texture_from_text(renderer, x, y, "\u2588", widget.style.fg);

	widget.text_input = t;
	return widget;
}

u8 text_input_handle_keydown(WORLD* w, EVENT e)
{
	TEXTURE* tex = &w->focus->text_input.tex;
	
}

// void widget_handle_keydown(WIDGET* w, EVENT e)
// {
	
// }

void widget_bind(WIDGET* w, char* key, u8(*fn)(WORLD*, EVENT))
{
	w->binds.binds[hash(key) % w->binds.size].custom = fn;
	SDL_Log("hash: %d", hash(key) % w->binds.size);
}


DEF_GET_MEMBER_ATTR_PTR_FN(SDL_Rect*, get_widget_rect_ptr, .tex.rect)

// SDL_Rect* get_widget_rect_ptr(WIDGET* w)
// {
	// switch (w->type)
	// {
		// case W_LABEL:
			// return &w->label.tex.rect;
		// break;
	// }

	// return &OUT_OF_BOUNDS_RECT;
// }


DEF_GET_MEMBER_ATTR_FN(SDL_Rect, get_widget_rect, .tex.rect, OUT_OF_BOUNDS_RECT)
// SDL_Rect get_widget_rect(WIDGET* w)
// {
	// return *get_widget_rect_ptr(w);
// }



DEF_GET_MEMBER_ATTR_PTR_FN(TEXTURE*, get_widget_textures, .tex)
TEXTURE* get_widget_texture_ptr(WIDGET* w)
{
	switch (w->type)
	{
		case W_LABEL:
			return &w->label.tex;
		break;
		case W_TEXT_INPUT:
			return &w->text_input.tex;
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