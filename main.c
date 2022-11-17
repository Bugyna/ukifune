#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <wchar.h>

#include "core.h"


#define FPS 60


#define WHITE (SDL_Color){255, 255, 255}

#define iterate(index, size, iterator, iterated) for (int index = 0; i < size; iterator = iterated[index++])


bool debugging = true;
bool run = true;
TTF_Font* font;



WORLD world;

void world_init(WORLD* w)
{
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
	// w->children = calloc(6, sizeof(WIDGET)); // this throws floating point exception somehow lmao

	WIDGET widget;
	widget.type = W_WORLD;
	widget.name = "WORLD";
	widget.world_parent = w;
	w->children[0] = widget;
	w->attention = &w->children[0];
	w->focus = &w->children[0];	
	w->style.font = font;
	w->style.bg = create_color_from_hex(0x050505);
	w->style.fg = create_color_from_hex(0xFFFFFF);
}

void alloc_world_hashmap(BINDS_HASHMAP* h)
{
	if (h->index % 10 == 0) {
		h->size += 10;
	}
	h->binds = realloc(h->binds, h->size*sizeof(BIND));
}

void alloc_world_children(WORLD* w)
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


void world_bind(WORLD* w, char* key, u8(*fn)(WIDGET*, EVENT))
{
	int type = 0;
	if (!strcmp(key, "<mousemove>")) {
		type = SDL_MOUSEMOTION;
	}

	w->binds.binds[hash(key) % w->binds.size].custom = fn;
	SDL_Log("hash: %d", hash(key) % w->binds.size);
}


void set_attention_auto(WORLD* w)
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
	if (strcmp(w->focus->name, tmp->name))
		widget_handle_attention_release(tmp);

	widget_handle_attention_get(w->attention);
}

void set_focus_auto(WORLD* w)
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
	if (strcmp(w->focus->name, tmp->name))
		widget_handle_focus_release(tmp);

	widget_handle_focus_get(w->focus);
}


void focus_set(WORLD* w, WIDGET* widget)
{
	w->focus = widget;
}

void focus_set_to_last_child(WORLD* w)
{
	w->focus = &w->children[w->child_index-1];
}


WIDGET* get_widget_by_name(WORLD* w, const char* name)
{
	for (int i = 0; i < w->child_index; i++)
		if (!strcmp(w->children[i].name, name))
			return &w->children[i];

	return NULL;
}

void focus_set_by_name(WORLD* w, const char* name)
{
	focus_set(w, get_widget_by_name(w, name));
}

WIDGET* focus_get(WORLD* w)
{
	return w->focus;
}

void world_handle_mouse_move(WORLD* w, EVENT e)
{
	u8(*fn)(WIDGET*, EVENT);
	// u8(*fn)(WIDGET*, EVENT) = get_bind_ptr(w->binds, "<mouse_move>");

	w->mouse_x = e.motion.x;
	w->mouse_y = e.motion.y;


	// SDL_Log("bef: %s", w->attention->name);
	set_attention_auto(w);
	// SDL_Log("aft: %s", w->attention->name);

	// if (fn != NULL)
		// if (fn(w, e))
			// return;
	
	
	fn = get_system_bind_ptr(w->binds, "<mouse_move>");
	if (fn != NULL)
		fn(w->focus, e);
		

}


void execute_widget_bind(WIDGET* w, char* bind_key, EVENT e)
{
	u8 res = 0;
	u8(*fn)(WIDGET*, EVENT);
	fn = get_bind_ptr(w->binds, bind_key);
	if (fn != NULL)
			res = fn(w, e);

	SDL_Log("custom widget bind res: %d", res);
	if (res)
		return;

	fn = get_system_bind_ptr(w->binds, bind_key);
	if (fn != NULL)
			fn(w, e);
}


void world_handle_mouse_button_down(WORLD* w, EVENT e)
{
	if (e.button.button == SDL_BUTTON_LEFT) {
		u8(*fn)(WIDGET*, EVENT) = get_bind_ptr(w->binds, "<mouse_left>");
	
		SDL_Log("focus bef: %s", w->focus->name);
		set_focus_auto(w);
		SDL_Log("focus aft: %s", w->focus->name);	
		execute_widget_bind(w->focus, "<mouse_left>", e);
	}
}

void world_handle_mouse_button_up(WORLD* w, EVENT e)
{
	
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

u8 world_handle_keydown(WORLD* w, EVENT e)
{
	// char s[2];
	// sprintf(s, "%c", e.key.keysym.sym);
	if (e.type == SDL_TEXTEDITING)
		SDL_Log("world keydown: %s", e.edit.text);
	else
		SDL_Log("world keydown: %s", e.text.text);
	// u8(*fn)(WORLD*, EVENT) = get_bind_ptr(w->binds, s);
	// if (fn == NULL)
		// return 0;

	execute_widget_bind(w->focus, "<keypress>", e);

	return 1;
}


u8 test_fn(WORLD* w, EVENT e) {
	printf("aaaaaaaa %d\n", e.type);
	return 0;
}


void cap_fps(int start_time)
{
	if (1000/FPS > SDL_GetTicks() - start_time) {
		SDL_Delay(1000/FPS - (SDL_GetTicks() - start_time));
	}
}



void world_add_child(WORLD* w, WIDGET widget)
{
	SDL_Log("adding child: %s %d", widget.name, w->child_index);
	w->children[w->child_index++] = widget;
	alloc_world_children(w);
}

void render(WORLD* w)
{
	SDL_SetRenderDrawColor(w->renderer, 0, 0, 0, 255);
	SDL_RenderClear(w->renderer);
	SDL_SetRenderDrawColor(w->renderer, 255, 0, 75, 255);

	for (int i = 1; i < w->child_index; i++) {
		// SDL_Log("child: %d", w->children[i].type);
		if (debugging)
			SDL_RenderDrawRect(w->renderer, get_widget_rect_ptr(&w->children[i]));


		if (w->children[i].render_fn != NULL)
			w->children[i].render_fn(&w->children[i]);

		else {
			TEXTURE* tex = get_widget_texture_ptr(&w->children[i]);
			if (tex == NULL) { SDL_Log("oh no a null fuckery"); continue; }
			// SDL_Log("aa: %s %p", w->children[i].name, get_widget_texture_ptr(&w->children[i]));
			SDL_RenderCopyEx(w->renderer, tex->tex, NULL, &tex->rect, 0, NULL, 0);
		}
	}

	SDL_RenderPresent(w->renderer);
}

int main () {
	if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) != (IMG_INIT_JPG | IMG_INIT_PNG)) {
		fprintf(stderr, "failed to initialize SDL IMG\n");
		return 1;
	}
	
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		fprintf(stderr, "failed to initialize SDL VIDEO\n");
		return 1;
	}
	
	if (TTF_Init()) {
		fprintf(stderr, "failed to initialize SDL TTF\n");
		return 1;
	}
	
	
	SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" );
	world_init(&world);
	
	SDL_Event event;
	bool moving = false;


	font = TTF_OpenFont("firacode.ttf", 18);
	world.style.font = font;

	uint32_t ticks = SDL_GetTicks();
	EVENT e;
	// e.type = 0;
	// (*world.fn[0])(&world, e);
	SDL_Log("dd: %s %ld", "シシ", sizeof(WIDGET));
	world_add_child(&world, create_label(&world, 200, 200, "test"));
	world_add_child(&world, create_label(&world, 400, 300, "áíýáí"));
	world_add_child(&world, create_label(&world, 600, 300, "áíýáídwad"));
	world_add_child(&world, create_text_input(&world, 500, 400, "atak"));
	// focus_set(&world, &world.children[4]);
	// focus_set_by_name(&world, "TEXT_INPUT4");
	focus_set_to_last_child(&world);
	SDL_SetTextInputRect(&world.children[4].text_input.tex.rect);
	SDL_Log("focus: %s\n", focus_get(&world)->name);
	// widget_bind(&world.children[3], "<keypress>", test_keypress);
	// world_bind(&world, "<mousemove>", test_mousemove);
	// exit(1);


	SDL_StartTextInput();
	while (run) {
		cap_fps(ticks);
		ticks = SDL_GetTicks();

		while (SDL_PollEvent(&event)) {
			// SDL_Log("event: %d", event.type);
			switch (event.type) {
				case SDL_QUIT:
					run = false;
				break;

				case SDL_MOUSEMOTION:
					world_handle_mouse_move(&world, event);
				break;

				case SDL_MOUSEBUTTONDOWN:
					world_handle_mouse_button_down(&world, event);
				break;
	
				// case SDL_KEYDOWN:
					// world_handle_keydown(&world, event);
				// break;

				case SDL_TEXTINPUT:
					world_handle_keydown(&world, event);
					// SDL_Log("textinput: %s %ld", event.text.text, strlen(event.text.text));
				break;

				case SDL_TEXTEDITING:
					SDL_Log("textedit: %s %d %d", event.edit.text, event.edit.start, event.edit.length);
					world_handle_keydown(&world, event);
				break;

				case SDL_KEYUP:
				break;
			}
			render(&world);
		}
	}


	SDL_DestroyRenderer(world.renderer);
	SDL_DestroyWindow(world.win);

	TTF_CloseFont(font);

	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
	return 0;
}
