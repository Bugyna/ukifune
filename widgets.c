#pragma once
#include "widgets.h"
// bool win_check_collision(WIN* w, 



DEF_GET_WIDGET_ATTR_PTR_FN(TEXTURE*, get_widget_textures, .tex)
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

DEF_GET_WIDGET_ATTR_PTR_FN(SDL_Rect*, get_widget_rect_ptr, .tex.rect)
DEF_GET_WIDGET_ATTR_FN(SDL_Rect, get_widget_rect, .tex.rect, OUT_OF_BOUNDS_RECT)

void widget_bind_system(WIDGET* w, char* bind, u8(*fn)BIND_FN_PARAMS)
{
	w->binds.binds[hash(bind) % w->binds.size].system = fn;
	SDL_Log("hash: %d", hash(bind) % w->binds.size);
}

void widget_bind(WIDGET* w, char* bind, u8(*fn)BIND_FN_PARAMS)
{
	w->binds.binds[hash(bind) % w->binds.size].custom = fn;
	SDL_Log("hash: %d", hash(bind) % w->binds.size);
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


char* generate_widget_name(WIN* w, WIDGET* widget)
{
	char* name = malloc(32);
	strcpy(name, widget_type_name(*widget));
	char* num = malloc(12);
	sprintf(num, "%d", w->child_index);
	strcat(name, num);
    
	return name;
}


void widget_init(WIN* w, WIDGET* widget, int type)
{
	widget->win_parent = w;
	widget->style = w->style;
	widget->type = type;
	widget->name = generate_widget_name(w, widget);

	widget->last_click_rx = 0;
	widget->last_click_ry = 0;
    
	widget->binds.size = 20;
	widget->binds.index = 0;
	widget->binds.binds = calloc(widget->binds.size, sizeof(BIND));
    
	widget->child_index = 0;
	widget->children = calloc(5, sizeof(WIDGET));
	widget->render_fn = NULL;
}

void widget_localize_mouse_location(WIDGET* w, EVENT* e)
{
	// make x and y of mouse location relative to the position of the widgets
	SDL_Rect rect = get_widget_rect(w);
	e->motion.x = abs(rect.x - e->motion.x);
	e->motion.y = abs(rect.y - e->motion.y);
	SDL_Log("localize mouse: %d %d", e->motion.x, e->motion.y);
}


WIDGET create_label(WIN* w, int x, int y, char* text)
{
	SDL_Log("CREATING LABEL %s", text);
	WIDGET widget;
	widget_init(w, &widget, W_LABEL);
	LABEL l;
	l.text.size = 64;
	l.text.index = 0;
	l.text.str = malloc(l.text.size);
	strcpy(l.text.str, text);

	l.tex = create_texture_from_text(w, x, y, text, widget.style.fg);
	SDL_Log("ddddddd");
    
	widget.label = l;
	return widget;
}



u8 text_input_handle_keydown BIND_FN_PARAMS
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
    
	w->text_input.tex = create_texture_from_text(w->win_parent, w->text_input.tex.rect.x, w->text_input.tex.rect.y, w->text_input.text.str, w->style.fg);
	w->text_input.cursor.rect.x = w->text_input.tex.rect.x + w->text_input.tex.rect.w;
	return 0;
}


void render_text_input(WIDGET* w)
{
	SDL_RenderCopy(w->win_parent->renderer, w->text_input.tex.tex, NULL, &w->text_input.tex.rect);
	SDL_RenderCopy(w->win_parent->renderer, w->text_input.cursor.tex, NULL, &w->text_input.cursor.rect);
}


WIDGET create_text_input(WIN* w, int x, int y, char* text)
{
	WIDGET widget;
	widget_init(w, &widget, W_TEXT_INPUT);
	TEXT_INPUT t;
    
	t.text.size = 64;
	t.text.index = strlen(text);
	t.text.str = malloc(t.text.size);
	strcpy(t.text.str, text);
	t.tex = create_texture_from_text(w, x, y, text, widget.style.fg);
	t.cursor = create_texture_from_text(w, x+t.tex.rect.w, y, "\u2588", widget.style.fg);
	// int tmp_w = 0, tmp_h = 0;
	// SDL_Log("motheffucker: %d", TTF_SizeUTF8(w->style.font, "\u2588\u2588\u2588\u2588", &tmp_w, &tmp_h));
	// t.cursor = create_texture_from_image(w->renderer, x+t.tex.rect.w, y, tmp_w, tmp_h, "maybe.png");
	widget_bind_system(&widget, "<keypress>", text_input_handle_keydown);
    
	widget.text_input = t;
	widget.render_fn = render_text_input;
	return widget;
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

