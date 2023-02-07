#pragma once
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "widgets.h"

#define FPS 60
#define WHITE (SDL_Color){255, 255, 255}
#define iterate(index, size, iterator, iterated) for (int index = 0; i < size; iterator = iterated[index++])


int FPS_COUNT, FRAMES_ELAPSED, DELTA_TIME, ONE_SECOND_DELIMETER;

void win_render_default(WIN* win);


void win_init(WIN* w)
{
	FPS_COUNT = 0;
	FRAMES_ELAPSED = 0;
	DELTA_TIME = 0;
	ONE_SECOND_DELIMETER = 0;

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

	SDL_SetRenderDrawBlendMode(w->renderer,
                               SDL_BLENDMODE_BLEND);


	SDL_GetWindowSize(w->win, &w->width, &w->height);
	
	w->binds.size = 20;
	w->binds.index = 0;
	w->binds.binds = calloc(w->binds.size, sizeof(BIND));
    
	w->child_index = 1;
	w->children = calloc(20, sizeof(WIDGET));
	
	w->render_index = 0;
	w->widget_render_list = calloc(20, sizeof(WIDGET*));

	PRIMITIVE_LIST_INIT(&w->primitive_list);
	// w->children = calloc(6, sizeof(WIDGET)); // this throws floating point exception somehow lmao
   
	global_font = TTF_OpenFont("Ac437_9x8.ttf", 18);
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

u8 (*get_bind_ptr(BINDS_HASHMAP h, char* bind))(WIDGET*, EVENT, char*)
{
	// SDL_Log("hash: %d", hash(bind) % h.size);
	// SDL_Log("custom bind ptr: %p", h.binds[hash(bind) % h.size].custom);
	return h.binds[hash(bind) % h.size].custom;
}

u8 (*get_system_bind_ptr(BINDS_HASHMAP h, char* bind))(WIDGET*, EVENT, char*)
{
	// SDL_Log("hash: %d", hash(bind) % h.size);
	return h.binds[hash(bind) % h.size].system;
}


BIND* get_bind(BINDS_HASHMAP h, char* bind)
{
	
	BIND* b = &h.binds[hash(bind) % h.size];
	// if (b->bind == NULL) return b;

	// SDL_Log("getbind: %s", b->bind);
	if (b != NULL && b->bind != NULL)
	{
		if (!strcmp(bind, b->bind)) {SDL_Log("jawdlwdakjh"); return b;}
		a:
		if (strcmp(b->bind, bind)) {
			if (b->next == NULL) return b;
			if (strcmp("<mouse_move>", bind) == 0) SDL_Log("dwawda %s %s %s", b->bind, bind, b->next->bind);
			b = b->next;
			if (!strcmp(bind, b->bind)) {SDL_Log("hapepebnd"); return b;}
			goto a;
		}
	
		// SDL_Log("dwawda ret %s %s", b->bind, bind);
		return b;
	}

	return b;
}


EVENT get_last_event(BINDS_HASHMAP h, char* bind)
{
	return h.binds[hash(bind) % h.size].last_event;
}


void win_bind(WIN* w, char* bind, u8(*fn)BIND_FN_PARAMS)
{
	// int type = 0;
	// if (!strcmp(bind, "<mousemove>")) {
		// type = SDL_MOUSEMOTION;
	// }

	w->binds.binds[hash(bind) % w->binds.size].bind = bind;
	w->binds.binds[hash(bind) % w->binds.size].custom = fn;
	SDL_Log("hash: %d", hash(bind) % w->binds.size);
}


void set_attention_auto(WIN* w)
{
	if (w->mouse_x > w->width || w->mouse_x < 0 || w->mouse_y > w->height || w->mouse_y < 0) return;
	WIDGET* tmp = w->attention;
	// WIDGET iterator;
	// iterate(i, w->child_index, iterator, w->children) {
	for (int i = 1; i < w->child_index; i++) {
		if (collide_point(w->mouse_x, w->mouse_y, get_widget_rect(&w->children[i]))) {
			if (w->attention != &w->children[0] && collide_point(w->mouse_x, w->mouse_y, get_widget_rect(w->focus))) return; // focused widgets get preffered
			w->attention = &w->children[i];
			return;
		}
	}
	w->attention = &w->children[0];
	// if (strcmp(w->focus->name, tmp->name))
		// widget_handle_attention_release(tmp);
    
	// widget_handle_attention_get(w->attention);
}

void set_focus_auto(WIN* w)
{
	if (w->mouse_x > w->width || w->mouse_x < 0 || w->mouse_y > w->height || w->mouse_y < 0) return;
	WIDGET* tmp = w->focus;
	for (int i = 1; i < w->child_index; i++) {
		// SDL_Log("idnex: %d %d %d", w->child_index, w->mouse_x, w->mouse_y);
		if (collide_point(w->mouse_x, w->mouse_y, get_widget_rect(&w->children[i]))) {
			// SDL_Log("here: %s %d %d %d %d", w->children[i].name, get_widget_rect(&w->children[i]).x, get_widget_rect(&w->children[i]).y, get_widget_rect(&w->children[i]).x + get_widget_rect(&w->children[i]).w, get_widget_rect(&w->children[i]).y+get_widget_rect(&w->children[i]).h);
			w->focus = &w->children[i];
			return;
		}
	}

	// if (collide_point(w->mouse_x, w->mouse_y, get_widget_rect(w->focus))) {
		// return;
	// }
	w->focus = &w->children[0];
	// if (strcmp(w->focus->name, tmp->name))
		// widget_handle_focus_release(tmp);
    
	// widget_handle_focus_get(w->focus);
}

void attention_set(WIN* w, WIDGET* widget)
{
	w->attention = widget;
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


u8 check_widget_bind(WIDGET* w, EVENT e, char* bind)
{
	u8(*fn)BIND_FN_PARAMS;
	BIND* b = get_bind(w->binds, bind);

	fn = b->custom;
	if (fn != NULL) {
		return 1;
		// w->win_parent->focus = w;
	}
    
	fn = b->system;
	if (fn != NULL)
		return 1;

	return 0;
}

void execute_widget_bind(WIDGET* w, EVENT e, char* bind)
{
	SDL_Log("execute widget bind %s\n", bind);
	u8 res = 0;
	u8(*fn)BIND_FN_PARAMS;
	BIND* b = get_bind(w->binds, bind);
	// fn = get_bind_ptr(w->binds, bind_key);
	// SDL_Log("execute widg
	if (strcmp(bind, "<mouse_move>")) {
		b->last_event = e;
		SDL_Rect rect = get_widget_rect(w);
		b->x = w->win_parent->mouse_x;
		b->y = w->win_parent->mouse_y;
		b->rx = abs(rect.x - w->win_parent->mouse_x);
		b->ry = abs(rect.y - w->win_parent->mouse_y);
		// SDL_Log("execute widget bind: %s %d %d %d %d", bind, w->win_parent->mouse_x, w->win_parent->mouse_y, b->rx, b->ry);
		// SDL_Log("rel: %d %d", b->rx, b->ry);
	}
	

	fn = b->custom;
	if (fn != NULL) {
		focus_set(w->win_parent, w);
		// w->win_parent->focus = w;
		res = fn(w, e, bind);
	}
    
	// SDL_Log("custom widget bind res: %d", res);
	// w->win_parent->lock = w;
	if (res)
		return;
    
	// fn = get_system_bind_ptr(w->binds, bind_key);
	fn = b->system;
	if (fn != NULL)
        fn(w, e, bind);
}

BIND** get_binds_from_key(BINDS_HASHMAP h, const char* bind)
{
	BIND** ret = calloc(5, sizeof(BIND*));
	int i = 0;
	char** x = get_bind_names_from_key(bind);

	for (int iter = 0; iter < 5; iter++) {
		if (x[iter] == NULL) break;
		ret[i++] = get_bind(h, x[iter]);
		SDL_Log("get binds from key: %s %s", x[iter], ret[i-1]->bind);
	}
	free(x);

	// char* tmp = malloc(30);
	
	// int len = strlen(bind), size = 0, offset = 0, i = 0;
	// SDL_Log("len: %d", len);
	// for ( ; offset < len; offset++)
	// {
		// tmp[size] = '\0';
		// // SDL_Log("tmp: %s %d	", tmp, offset);
		// if (size) tmp[size++] = bind[offset];
		// if (bind[offset] == '<') tmp[size++] = bind[offset];
		// else if (bind[offset] == '>') {
			// tmp[size] = '\0';
			// ret[i++] = get_bind(h, tmp);
			// // SDL_Log("tmp end: %s", tmp);
			// size = 0;
		// }
	// }


	// free(tmp);
	return ret;
}


// BIND* parse_bind_key(char* bind)
// {
	// BIND* ret = calloc(5*sizeof(BIND));
	// int len = strlen(bind), size = 0, offset = 0;
	// for (int i = 0; offset < len;)
	// {
		// size = strcspn(bind+offset, ">");
		// SDL_Log("size: %d %d %s", size, offset, bind+offset);
		// offset += size+1;
		// // ret[i] = malloc(size+1);
		// ret[i++] = get_bind(bind+offset);
		// // strncpy(ret[i++], bind+offset, size);
	// }

	// return ret;
// }


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

	// SDL_Log("win_handle_mouse move e.button.button: %d %d", e.motion.state, SDL_BUTTON_RMASK);
	// widget_localize_mouse_location(w->focus, &e);
	char* tmp = malloc(128);

	
	int o = 0, n = 0;
	uint8_t* keys = SDL_GetKeyboardState(&n);
	for (int i = 0; i < n; i++)
		// if (keys[i] == 1) SDL_Log("key: %s %i", SDL_GetScancodeName(i), n);
		// if (keys[i] == 1) SDL_Log("key: %c %i", SDL_GetKeyFromScancode(i), n);
		if (keys[i] == 1) tmp[o++] = SDL_GetKeyFromScancode(i);

	// tmp[o++] = '\0';
	// SDL_Log("dwaww: %s", tmp);

	// SDL_Log("attention: %s %s", w->attention->name, w->focus->name);
	// if (w->lock == w->focus)
	// if (w->attention == w->focus)
	// {
		switch (e.motion.state)
		{
			case SDL_BUTTON_LMASK:
				strcpy(tmp+o, "<mouse_left>");
				// execute_widget_bind(w->focus, e, "<mouse_left><mouse_move>");
				o += 12;
				break;
			case SDL_BUTTON_RMASK:
				strcpy(tmp+o, "<mouse_right>");
				o += 13;
				// execute_widget_bind(w->focus, e, "<mouse_right><mouse_move>");
				break;
			case SDL_BUTTON_MMASK:
				// SDL_Log("mouse middle move %d %d", SDL_BUTTON_MMASK, e.motion.state);
				// strcpy(tmp+o, "<mouse_middle><mouse_move>");
				strcpy(tmp+o, "<mouse_middle>");
				o += 14;
				// o += 28;
				// execute_widget_bind(w->focus, e, "<mouse_middle><mouse_move>");
				break;
			// default: execute_widget_bind(w->focus, e, "<mouse_move>");
		}
		// else execute_widget_bind(w->focus, e, "<mouse_move>");
		// SDL_Log("Dwawd: %d", o);
		strcpy(tmp+o, "<mouse_move>");
		tmp[o+13] = '\0';
		// SDL_Log("dwa: %s %d", tmp, e.motion.state);
		execute_widget_bind(w->focus, e, tmp);
		if (w->focus != w->attention && check_widget_bind(w->attention, e, tmp) && strcmp(tmp, "<mouse_move>")) {
			execute_widget_bind(w->attention, e, tmp);
		}
	// }

	
}


void win_handle_mouse_button_down(WIN* w, EVENT e)
{
	// u8(*fn)BIND_FN_PARAMS;

	// widget_localize_mouse_location(w->focus, &e);
	char* tmp = NULL;
	SDL_Log("win mouse button down e.button.button: %d %d %d", e.button.button, SDL_BUTTON_RIGHT, e.motion.x);
	switch (e.button.button)
	{
		case SDL_BUTTON_LEFT:
			SDL_Log("mouse left press");
			tmp = "<mouse_left>";
			break;
		case SDL_BUTTON_RIGHT:
			tmp = "<mouse_right>";
			break;
		case SDL_BUTTON_MIDDLE:
			SDL_Log("mouse middle press");
			tmp = "<mouse_middle>";
			break;
	}

	execute_widget_bind(w->focus, e, tmp);
	if (w->focus != w->attention) {
		execute_widget_bind(w->attention, e, tmp);
	}

	
	// if (e.button.button == SDL_BUTTON_LEFT) {
		// u8(*fn)(WIDGET*, EVENT, char*) = get_bind_ptr(w->binds, "<mouse_left>");
		// w->last_click = e;
        
		// SDL_Log("focus bef: %s", w->focus->name);
		// set_focus_auto(w);
		// w->focus->last_click = e;
		// TEXTURE* t = get_widget_texture_ptr(w->focus);
		// w->focus->last_click_rx = abs(t->rect.x - e.motion.x);
		// w->focus->last_click_ry = abs(t->rect.y - e.motion.y);
		// SDL_Log("focus aft: %s", w->focus->name);
		// execute_widget_bind(w->focus, e, "<mouse_left>");
	// }
	// execute_widget_bind(w->focus, e, "<mouse_left>");
}

void win_handle_mouse_button_up(WIN* w, EVENT e)
{
	if (e.button.button == SDL_BUTTON_LEFT) {
		u8(*fn)(WIDGET*, EVENT, char*) = get_bind_ptr(w->binds, "<mouse_left_release>");
		w->last_click_release = e;
        
		SDL_Log("focus bef: %s", w->focus->name);
		set_focus_auto(w);
		SDL_Log("focus aft: %s", w->focus->name);
		// w->focus->last_click_release = e;
		execute_widget_bind(w->focus, e, "<mouse_left_release>");
		// execute_widget_bind(w->attention, e, "<mouse_left_release>");
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
	// char* a = malloc(3);

	switch (e.type)
	{
		case SDL_KEYDOWN:
			// SDL_Log("d: %c", e.key.keysym.sym);
			// a[0] = e.key.keysym.sym;
			// a[1] = '\0';
			break;
			
		// case SDL_TEXTEDITING:
			// a = e.edit.text;
			// break;

		// default:
			// a = e.text.text;
	}
	// SDL_Log("win keydown: %s %d", a, e.key.repeat);
	// if (e.type == SDL_TEXTEDITING) 
		// SDL_Log("win keydown: %s", e.edit.text);
	// else
		// SDL_Log("win keydown: %s", e.text.text);
	// u8(*fn)(WIN*, EVENT) = get_bind_ptr(w->binds, s);
	// if (fn == NULL)
    // return 0;

	char* tmp = malloc(20);
	tmp[0] = SDL_GetKeyFromScancode(e.key.keysym.sym);
	tmp[1] = '\0';
	
	// tmp[0] = '<';
	// tmp[1] = e.key.keysym.sym;
	// tmp[2] = '>';
	// tmp[3] = '\0';
	// SDL_Log("dd: %s", tmp);
	
	if (!e.key.repeat) {
		// execute_widget_bind(w->focus, e, "<keypress>");
		execute_widget_bind(w->focus, e, tmp);
		// SDL_Log("handle keypress: %s", tmp);
	}
	// else execute_widget_bind(w->focus, e, "<keydown>");
    
	return 1;
}

u8 win_handle_keypress(WIN* w, EVENT e)
{
	// char s[2];
	// sprintf(s, "%c", e.key.keysym.sym);
	char* a = malloc(2);

	switch (e.type)
	{
		case SDL_KEYDOWN:
			a[0] = e.key.keysym.sym;
			a[1] = '\0';
			break;
			
		case SDL_TEXTEDITING:
			a = e.edit.text;
			break;

		default:
			a = e.text.text;
	}

	char tmp[4] = {'<', a[0], '>', '\0'};
	
	if (!e.key.repeat) {
		execute_widget_bind(w->focus, e, "<keypress>");
		execute_widget_bind(w->focus, e, "<keypress>");
		SDL_Log("handle keypress: %s", tmp);
	}
	else execute_widget_bind(w->focus, e, "<keydown>");
    
	return 1;
}


u8 test_fn(WIN* w, EVENT e) {
	// printf("aaaaaaaa %d\n", e.type);
	return 0;
}


void cap_fps(uint32_t* start_time)
{
	FRAMES_ELAPSED++;
	DELTA_TIME = SDL_GetTicks() - *start_time;
	if (ONE_SECOND_DELIMETER < SDL_GetTicks() - 1000.0) {
		FPS_COUNT = FRAMES_ELAPSED;
		FRAMES_ELAPSED = 0;
		ONE_SECOND_DELIMETER = SDL_GetTicks();
	}
	// *start_time = SDL_GetTicks();
		// SDL_Log("ticks: %u %u %u", SDL_GetTicks(), *start_time, (SDL_GetTicks() - *start_time));
		// FPS_COUNT = SDL_GetTicks() - *start_time;
		// SDL_Delay((SDL_GetTicks() - *start_time));
}


void cap_fps_fixed(uint32_t* start_time, uint32_t period)
{
	FRAMES_ELAPSED++;
	uint32_t x = SDL_GetTicks();
	// SDL_Log("ticks: %u %u %u", SDL_GetTicks(), *start_time, (SDL_GetTicks() - *start_time));
	if (x/period > SDL_GetTicks() - *start_time) {
	// if (period > SDL_GetTicks() - *start_time) {
		SDL_Delay(x/period - (x - (*start_time)));
		// SDL_Delay((SDL_GetTicks() - *start_time));

		*start_time = SDL_GetTicks();
		FPS_COUNT = FRAMES_ELAPSED;
		FRAMES_ELAPSED = 0;
	}
}

// void cap_fps(uint32_t *start_time)
// {
	// FRAMES_ELAPSED++;
		// SDL_Log("ticks: %u %u %u", SDL_GetTicks(), *start_time, (SDL_GetTicks() - *start_time));
	// if (1000/FPS > SDL_GetTicks() - *start_time) {
		// // FPS_COUNT = SDL_GetTicks() - *start_time;
		// // SDL_Delay((SDL_GetTicks() - *start_time));

		// FPS_COUNT = SDL_GetTicks() - *start_time;
		// *start_time = SDL_GetTicks();
		// // FPS_COUNT = FRAMES_ELAPSED;
		// FRAMES_ELAPSED = 0;
	// }
// }

// void cap_fps(uint32_t* start_time)
// {
	// FRAMES_ELAPSED++;
	// if (*start_time < SDL_GetTicks() - 1000.0) {
		// SDL_Log("ticks: %u %u %u", SDL_GetTicks(), *start_time, (SDL_GetTicks() - *start_time));
		// *start_time = SDL_GetTicks();
		// // SDL_Delay(1000/FPS - (SDL_GetTicks() - *start_time));
		// FPS_COUNT = FRAMES_ELAPSED;
		// FRAMES_ELAPSED = 0;
	// }
// }



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

	// SDL_Log("aaa: %d %d", w->primitive_list.last->p.r.x, w->primitive_list.last->p.r.h);
	for (PRIMITIVE_NODE* n = w->primitive_list.first; (n != NULL && n != n->next); n = n->next)
	{
		SDL_SetRenderDrawColor(w->renderer, 150, 150, 150, 55);
		if (n->p.type == P_RECT) {
			SDL_RenderDrawRect(w->renderer, &n->p.r);
			SDL_SetRenderDrawColor(w->renderer, 150, 150, 150, 22);
			SDL_RenderFillRect(w->renderer, &n->p.r);
		}
	}
	SDL_RenderPresent(w->renderer);
}


void win_handle_resize(WIN* w, EVENT e)
{
	SDL_GetWindowSize(w->win, &w->width, &w->height);
}


void win_handle_window_event(WIN* w, EVENT e)
{
	switch (e.window.event)
	{
		case SDL_WINDOWEVENT_RESIZED:
			win_handle_resize(w, e);
		break;
	}
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

u8 win_close(WIDGET* w, EVENT e)
{
	uki_destroy_win(w->win_parent);
	return 0;
}



