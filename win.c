#pragma once
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "widgets.h"

#define FPS 60
#define WHITE (SDL_Color){255, 255, 255}
#define iterate(index, size, iterator, iterated) for (int index = 0; i < size; iterator = iterated[index++])

void win_render_default(WIN* win);

void win_init(WIN* w)
{

	if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) != (IMG_INIT_JPG | IMG_INIT_PNG)) {
		fprintf(stderr, "failed to initialize SDL IMG\n");
		exit(1);
	}
	
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		fprintf(stderr, "failed to initialize SDL VIDEO\n");
		exit(1);
	}
	
	if (TTF_Init()) {
		fprintf(stderr, "failed to initialize SDL TTF\n");
		exit(1);
	}
	
	
	SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" );


	w->win = SDL_CreateWindow(
                              "test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              1280, 720, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE|SDL_WINDOW_ALLOW_HIGHDPI
                              );
	
	w->renderer = SDL_CreateRenderer(w->win, -1, SDL_RENDERER_ACCELERATED);
	
	w->binds.size = 20;
	w->binds.index = 0;
	w->binds.binds = calloc(w->binds.size, sizeof(BIND));
    
	w->child_index = 1;
	w->children = calloc(5, sizeof(WIDGET));
	
	w->render_index = 0;
	w->render_list = calloc(5, sizeof(WIDGET*));
	// w->children = calloc(6, sizeof(WIDGET)); // this throws floating point exception somehow lmao
   
	global_font = TTF_OpenFont("firacode.ttf", 18);
	w->style.font = global_font;
	
	WIDGET widget = create_label(w, 1280, 720, "WIN");
	widget.type = W_WIN;
	widget.name = "WIN";
	widget.win_parent = w;
	w->children[0] = widget;
	w->attention = &w->children[0];
	w->focus = &w->children[0];
	w->style.font = font;
	w->style.bg = create_color_from_hex(0x050505);
	w->style.fg = create_color_from_hex(0xFFFFFF);
	// w->render_fn = win_render_default;
}

void alloc_win_hashmap(BINDS_HASHMAP* h)
{
	if (h->index % 10 == 0) {
		h->size += 10;
	}
	h->binds = realloc(h->binds, h->size*sizeof(BIND));
}

void alloc_win_children(WIN* w)
{
	if (w->child_index % 5 == 0) {
		// w->child_index += 5;
		w->children = realloc(w->children, (w->child_index+5)*sizeof(WIDGET));
	}
}

u8 (*get_bind_ptr(BINDS_HASHMAP h, char* key))(WIDGET*, EVENT)
{
	// SDL_Log("hash: %d", hash(key) % h.size);
	// SDL_Log("custom bind ptr: %p", h.binds[hash(key) % h.size].custom);
	return h.binds[hash(key) % h.size].custom;
}

u8 (*get_system_bind_ptr(BINDS_HASHMAP h, char* key))(WIDGET*, EVENT)
{
	// SDL_Log("hash: %d", hash(key) % h.size);
	return h.binds[hash(key) % h.size].system;
}


BIND* get_bind(BINDS_HASHMAP h, char* key)
{
	return &h.binds[hash(key) % h.size];
}


EVENT get_last_event(BINDS_HASHMAP h, char* key)
{
	return &h.binds[hash(key) % h.size].last_event;
}


void win_bind(WIN* w, char* key, u8(*fn)(WIDGET*, EVENT))
{
	// int type = 0;
	// if (!strcmp(key, "<mousemove>")) {
		// type = SDL_MOUSEMOTION;
	// }

	w->binds.binds[hash(key) % w->binds.size].key = key;
	w->binds.binds[hash(key) % w->binds.size].custom = fn;
	SDL_Log("hash: %d", hash(key) % w->binds.size);
}


void set_attention_auto(WIN* w)
{
	WIDGET* tmp = w->attention;
	// WIDGET iterator;
	// iterate(i, w->child_index, iterator, w->children) {
	for (int i = 1; i < w->child_index; i++) {
		if (collide_point(w->mouse_x, w->mouse_y, get_widget_rect(&w->children[i]))) {
			w->attention = &w->children[i];
			return;
		}
	}
    
	w->attention = &w->children[0];
	// if (strcmp(w->focus->name, tmp->name))
		// widget_handle_attention_release(tmp);
    
	widget_handle_attention_get(w->attention);
}

void set_focus_auto(WIN* w)
{
	WIDGET* tmp = w->focus;
	for (int i = 1; i < w->child_index; i++) {
		// SDL_Log("idnex: %d %d %d", w->child_index, w->mouse_x, w->mouse_y);
		if (collide_point(w->mouse_x, w->mouse_y, get_widget_rect(&w->children[i]))) {
			// SDL_Log("here: %s %d %d %d %d", w->children[i].name, get_widget_rect(&w->children[i]).x, get_widget_rect(&w->children[i]).y, get_widget_rect(&w->children[i]).x + get_widget_rect(&w->children[i]).w, get_widget_rect(&w->children[i]).y+get_widget_rect(&w->children[i]).h);
			w->focus = &w->children[i];
			return;
		}
	}
	w->focus = &w->children[0];
	// if (strcmp(w->focus->name, tmp->name))
		// widget_handle_focus_release(tmp);
    
	widget_handle_focus_get(w->focus);
}


void focus_set(WIN* w, WIDGET* widget)
{
	w->focus = widget;
}

void focus_set_to_last_child(WIN* w)
{
	w->focus = &w->children[w->child_index-1];
}


WIDGET* get_widget_by_name(WIN* w, const char* name)
{
	for (int i = 0; i < w->child_index; i++)
		if (!strcmp(w->children[i].name, name))
        return &w->children[i];
    
	return NULL;
}

void focus_set_by_name(WIN* w, const char* name)
{
	focus_set(w, get_widget_by_name(w, name));
}

WIDGET* focus_get(WIN* w)
{
	return w->focus;
}

void execute_widget_bind(WIDGET* w, char* bind_key, EVENT e)
{
	u8 res = 0;
	u8(*fn)(WIDGET*, EVENT);
	BIND* b = get_bind(w->binds, bind_key);
	// fn = get_bind_ptr(w->binds, bind_key);
	b->last_event = e;

	fn = b->custom;
	if (fn != NULL)
        res = fn(w, e);
    
	// SDL_Log("custom widget bind res: %d", res);
	if (res)
		return;
    
	// fn = get_system_bind_ptr(w->binds, bind_key);
	fn = b->system;
	if (fn != NULL)
        fn(w, e);
}

void win_handle_mouse_move(WIN* w, EVENT e)
{
	// u8(*fn)(WIDGET*, EVENT);
	// u8(*fn)(WIDGET*, EVENT) = get_bind_ptr(w->binds, "<mouse_move>");
    
	w->mouse_x = e.motion.x;
	w->mouse_y = e.motion.y;
    
    
	// SDL_Log("bef: %s", w->attention->name);
	set_attention_auto(w);
	// SDL_Log("aft: %s", w->attention->name);
    
	// if (fn != NULL)
    // if (fn(w, e))
    // return;
	

	switch (e.button.button)
	{
		case SDL_BUTTON_LEFT:
			execute_widget_bind(w->focus, "<mouse_left_drag>", e);
			break;
		case SDL_BUTTON_RIGHT:
			execute_widget_bind(w->focus, "<mouse_right_drag>", e);
			break;
		case SDL_BUTTON_MIDDLE:
			execute_widget_bind(w->focus, "<mouse_middle_drag>", e);
			break;
		default: execute_widget_bind(w->focus, "<mouse_move>", e);
	}
}


void win_handle_mouse_button_down(WIN* w, EVENT e)
{
	if (e.button.button == SDL_BUTTON_LEFT) {
		u8(*fn)(WIDGET*, EVENT) = get_bind_ptr(w->binds, "<mouse_left>");
		w->last_click = e;
        
		SDL_Log("focus bef: %s", w->focus->name);
		// set_focus_auto(w);
		// w->focus->last_click = e;
		TEXTURE* t = get_widget_texture_ptr(w->focus);
		w->focus->last_click_rx = abs(t->rect.x - e.motion.x);
		w->focus->last_click_ry = abs(t->rect.y - e.motion.y);
		SDL_Log("focus aft: %s", w->focus->name);	
		execute_widget_bind(w->focus, "<mouse_left>", e);
	}
}

void win_handle_mouse_button_up(WIN* w, EVENT e)
{
	if (e.button.button == SDL_BUTTON_LEFT) {
		u8(*fn)(WIDGET*, EVENT) = get_bind_ptr(w->binds, "<mouse_left_release>");
		w->last_click_release = e;
        
		SDL_Log("focus bef: %s", w->focus->name);
		set_focus_auto(w);
		SDL_Log("focus aft: %s", w->focus->name);
		// w->focus->last_click_release = e;
		execute_widget_bind(w->focus, "<mouse_left_release>", e);
	}
}

u8 test_mousemove(WIDGET* w, EVENT e)
{
	// SDL_Log("<mouseclick> %d %d\n", w->mouse_x, w->mouse_y);
	return 1;
}

u8 test_keypress(WIDGET* w, EVENT e)
{
	// if (e.type == SDL_TEXTEDITING)
    // SDL_Log("custom keydown: %s", e.edit.text);
	// else
    // SDL_Log("custom keydown: %s", e.text.text);
	return 0;
}

u8 win_handle_keydown(WIN* w, EVENT e)
{
	// char s[2];
	// sprintf(s, "%c", e.key.keysym.sym);
	if (e.type == SDL_TEXTEDITING)
		SDL_Log("win keydown: %s", e.edit.text);
	else
		SDL_Log("win keydown: %s", e.text.text);
	// u8(*fn)(WIN*, EVENT) = get_bind_ptr(w->binds, s);
	// if (fn == NULL)
    // return 0;
    
	execute_widget_bind(w->focus, "<keypress>", e);
    
	return 1;
}


u8 test_fn(WIN* w, EVENT e) {
	printf("aaaaaaaa %d\n", e.type);
	return 0;
}


void cap_fps(int start_time)
{
	if (1000/FPS > SDL_GetTicks() - start_time) {
		SDL_Delay(1000/FPS - (SDL_GetTicks() - start_time));
	}
}



void win_add_child(WIN* w, WIDGET widget)
{
	SDL_Log("adding child: %s %d", widget.name, w->child_index);
	w->children[w->child_index++] = widget;
	alloc_win_children(w);
}

// void win_render_test(WIN* win)
// {
	// SDL_SetRenderDrawColor(win->renderer, 111, 0, 0, 255);
	// SDL_RenderClear(win->renderer);
	// SDL_SetRenderDrawColor(win->renderer, 255, 0, 75, 255);
	// SDL_Log("dddwad: %d", world.state);
	// for (int i = 0; i < world.render_list_index; i++)
	// {
		// TEXTURE* tex = get_component_texture_ptr(&world.render_list[i]->components[0]);
		// render_texture(win, tex);
	// }
    
	// SDL_RenderPresent(win->renderer);
// }

void win_render_default(WIN* w)
{
	SDL_SetRenderDrawColor(w->renderer, 0, 0, 0, 255);
	SDL_RenderClear(w->renderer);
	SDL_SetRenderDrawColor(w->renderer, 255, 0, 75, 255);

	for (int i = 1; i < w->child_index; i++) {
		if (debugging)
			SDL_RenderDrawRect(w->renderer, get_widget_rect_ptr(&w->children[i]));
        
        
		if (w->children[i].render_fn != NULL)
			w->children[i].render_fn(&w->children[i]);
        
		else {
			TEXTURE* tex = get_widget_texture_ptr(&w->children[i]);
			if (tex == NULL) { SDL_Log("oh no a null fuckery"); continue; }
			SDL_RenderCopyEx(w->renderer, tex->tex, NULL, &tex->rect, 0, NULL, 0);
		}
	}
	SDL_RenderPresent(w->renderer);
}


void uki_destroy_win(WIN* win)
{
	SDL_DestroyRenderer(win->renderer);
	SDL_DestroyWindow(win->win);
    
	TTF_CloseFont(win->style.font);
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}


