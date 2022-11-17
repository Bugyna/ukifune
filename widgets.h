#pragma once
#include "util.h"
#include "core.h"
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>


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
	u8(*system)(WIDGET*, EVENT);
	u8(*custom)(WIDGET*, EVENT);
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

struct WORLD
{
	SDL_Window* win;
	SDL_Renderer* renderer;

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

void widget_bind_system(WIDGET* w, char* key, u8(*fn)(WIDGET*, EVENT))
{
	w->binds.binds[hash(key) % w->binds.size].system = fn;
	SDL_Log("hash: %d", hash(key) % w->binds.size);
}

void widget_bind(WIDGET* w, char* key, u8(*fn)(WIDGET*, EVENT))
{
	w->binds.binds[hash(key) % w->binds.size].custom = fn;
	SDL_Log("hash: %d", hash(key) % w->binds.size);
}

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
	widget->render_fn = NULL;
}


WIDGET create_label(WORLD* w, int x, int y, char* text)
{
	WIDGET widget;
	widget_init(w, &widget, W_LABEL);
	LABEL l;
	l.text.size = 64;
	l.text.index = 0;
	l.text.str = malloc(l.text.size);
	strcpy(l.text.str, text);

	l.tex = create_texture_from_text(w->renderer, x, y, text, widget.style.fg);

	widget.label = l;
	return widget;
}



u8 text_input_handle_keydown(WIDGET* w, EVENT e)
{
	// TEXTURE* tex = &w->text_input.tex;
	// TODO NON LATIN INPUT
	// if (e.type == SDL_TEXTEDITING)
		// SDL_Log("system keydown: %s", e.edit.text);
	// else if (e.type == SDL_TEXTINPUT)
		// SDL_Log("system keydown: %s", e.text.text);

	// else {
		// SDL_Log("quitting");
		// exit(1);
	// }


	w->text_input.text.index += strlen(e.text.text);
	if (w->text_input.text.index >= w->text_input.text.size) {
		w->text_input.text.size += 64+strlen(e.text.text);
		w->text_input.text.str = realloc(w->text_input.text.str, w->text_input.text.size);
	}
	// SDL_Log("str debug: %d %d %ld", w->text_input.text.size, w->text_input.text.index, strlen(e.text.text));
	strcat(w->text_input.text.str, e.text.text);
	// w->text_input.text.str = e.text.text;
	// SDL_Log("idek: %s", w->text_input.text.str);

	w->text_input.tex = create_texture_from_text(w->world_parent->renderer, w->text_input.tex.rect.x, w->text_input.tex.rect.y, w->text_input.text.str, w->style.fg);
	w->text_input.cursor.rect.x = w->text_input.tex.rect.x + w->text_input.tex.rect.w;
	return 0;
}


void render_text_input(WIDGET* w)
{
	SDL_RenderCopy(w->world_parent->renderer, w->text_input.tex.tex, NULL, &w->text_input.tex.rect);
	SDL_RenderCopy(w->world_parent->renderer, w->text_input.cursor.tex, NULL, &w->text_input.cursor.rect);
}


WIDGET create_text_input(WORLD* w, int x, int y, char* text)
{
	WIDGET widget;
	widget_init(w, &widget, W_TEXT_INPUT);
	TEXT_INPUT t;

	t.text.size = 64;
	t.text.index = strlen(text);
	t.text.str = malloc(t.text.size);
	strcpy(t.text.str, text);
	t.tex = create_texture_from_text(w->renderer, x, y, text, widget.style.fg);
	t.cursor = create_texture_from_text(w->renderer, x+t.tex.rect.w, y, "\u2588", widget.style.fg);
	// int tmp_w = 0, tmp_h = 0;
	// SDL_Log("motheffucker: %d", TTF_SizeUTF8(w->style.font, "\u2588\u2588\u2588\u2588", &tmp_w, &tmp_h));
	// t.cursor = create_texture_from_image(w->renderer, x+t.tex.rect.w, y, tmp_w, tmp_h, "maybe.png");
	widget_bind_system(&widget, "<keypress>", text_input_handle_keydown);

	widget.text_input = t;
	widget.render_fn = render_text_input;
	return widget;
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