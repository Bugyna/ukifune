#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <wchar.h>

#include <SDL2/SDL_video.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>


#include "core.h"


#define FPS 60


#define WHITE (SDL_Color){255, 255, 255}

#define iterate(index, size, iterator, iterated) for (int index = 0; i < size; iterator = iterated[index++])


uint32_t hash(const char* key)
{
	// return FNV1A_Pippip_Yurii(key, strlen(key));
	return strlen(key);
}


bool debugging = true;
bool run = true;
TTF_Font* font;



WORLD world;

void world_init(WORLD* w)
{
	w->mouse.size = 10;
	w->mouse.index = 0;
	w->mouse.binds = calloc(10, sizeof(BIND));

	w->keydown.size = 10;
	w->keydown.index = 0;
	w->keydown.binds = calloc(10, sizeof(BIND));

	w->keyup.size = 10;
	w->keyup.index = 0;
	w->keyup.binds = calloc(10, sizeof(BIND));

	w->child_index = 1;
	w->children = calloc(5, sizeof(WIDGET));

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
	w->child_index++;
	if (w->child_index % 5 == 0) {
		w->child_index += 5;
	}
	w->children = realloc(w->children, w->child_index*sizeof(WIDGET));
}

u8 (*get_bind_ptr(BINDS_HASHMAP h, char* key))(WORLD*, EVENT)
{
	SDL_Log("hash: %d", hash(key) % h.size);
	return h.binds[hash(key) % h.size].custom;
}

u8 (*get_system_bind_ptr(BINDS_HASHMAP h, char* key))(WORLD*, EVENT)
{
	SDL_Log("hash: %d", hash(key) % h.size);
	return h.binds[hash(key) % h.size].system;
}


void world_bind(WORLD* w, char* key, u8(*fn)(WORLD*, EVENT))
{
	int type = 0;
	if (!strcmp(key, "<mousemove>")) {
		type = SDL_MOUSEMOTION;
	}


	w->mouse.binds[hash(key) % w->mouse.size].custom = fn;
	SDL_Log("hash: %d", hash(key) % w->mouse.size);
}


void set_attention_auto(WORLD* w)
{
	WIDGET* tmp = w->attention;
	// WIDGET iterator;
	// iterate(i, w->child_index, iterator, w->children) {
	for (int i = 0; i < w->child_index; i++) {
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
	for (int i = 0; i < w->child_index; i++) {
		if (collide_point(w->mouse_x, w->mouse_y, get_widget_rect(&w->children[i]))) {
			w->focus = &w->children[i];
			return;
		}
	}
	w->focus = &w->children[0];
	if (strcmp(w->focus->name, tmp->name))
		widget_handle_focus_release(tmp);

	widget_handle_focus_get(w->focus);
}


void world_handle_mouse_move(WORLD* w, EVENT e)
{
	u8(*fn)(WORLD*, EVENT) = get_bind_ptr(w->mouse, "<mouse_move>");

	w->mouse_x = e.motion.x;
	w->mouse_y = e.motion.y;


	SDL_Log("bef: %s", w->attention->name);
	set_attention_auto(w);
	SDL_Log("aft: %s", w->attention->name);

	if (fn != NULL)
		if (fn(w, e))
			return;
	
	
	fn = get_system_bind_ptr(w->mouse, "<mouse_move>");
	if (fn != NULL)
		fn(w, e);
		

}

void world_handle_mouse_button_down(WORLD* w, EVENT e)
{
	if (e.button.button == SDL_BUTTON_LEFT) {
		u8(*fn)(WORLD*, EVENT) = get_bind_ptr(w->mouse, "<mouse_left>");
	
		SDL_Log("focus bef: %s", w->focus->name);
		set_focus_auto(w);
		SDL_Log("focus aft: %s", w->focus->name);
	
		if (fn != NULL)
			fn(w, e);
	}
}

void world_handle_mouse_button_up(WORLD* w, EVENT e)
{
	
}


u8 test_mousemove(WORLD* w, EVENT e)
{
	SDL_Log("<mouseclick> %d %d %d %d\n", e.motion.x, e.motion.y, w->mouse_x, w->mouse_y);
	return 1;
}

u8 world_handle_keydown(WORLD* w, EVENT e)
{
	char s[2];
	sprintf(s, "%d", e.key.keysym.sym);
	u8(*fn)(WORLD*, EVENT) = get_bind_ptr(w->keydown, s);
	if (fn == NULL)
		return 0;

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
	w->children[w->child_index++] = widget;
	if (w->child_index % 5 == 0) {
		alloc_world_children(w);
	}
}

void render(SDL_Renderer* renderer, WORLD* w)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 255, 0, 75, 255);

	for (int i = 0; i < w->child_index; i++) {
		// SDL_Log("child: %d", w->children[i].type);
		if (debugging)
			SDL_RenderDrawRect(renderer, get_widget_rect_ptr(&w->children[i]));
		switch (w->children[i].type)
		{
			case W_LABEL:
				SDL_RenderCopy(renderer, w->children[i].label.tex.tex, NULL, &w->children[i].label.tex.rect);
			break;
		}
	}
	
	SDL_RenderPresent(renderer);
}

int main () {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		fprintf(stderr, "failed to initialize SDL VIDEO\n");
		return 1;
	}
	
	// if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) != 0) {
		// fprintf(stderr, "failed to initialize SDL IMG\n");
		// return 1;
	// }
	
	if (TTF_Init()) {
		fprintf(stderr, "failed to initialize SDL TTF\n");
		return 1;
	}
	
	
	SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" );
	world_init(&world);
	
	SDL_Window* window = SDL_CreateWindow(
		"test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		1280, 720, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE|SDL_WINDOW_ALLOW_HIGHDPI
	);
	
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_Event event;
	bool moving = false;

	// exit(1);

	font = TTF_OpenFont("firacode.ttf", 18);

	uint32_t ticks = SDL_GetTicks();
	EVENT e;
	// e.type = 0;
	// (*world.fn[0])(&world, e);
	SDL_Log("dd: %s", "シシ");
	world_add_child(&world, create_label(renderer, &world, 200, 200, "test"));
	world_add_child(&world, create_label(renderer, &world, 400, 300, "áíýáí"));
	world_bind(&world, "<mousemove>", test_mousemove);

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
	
				case SDL_KEYDOWN:
					// if (event.key.keysym.sym == ' ')
					SDL_Log("key: %d", event.key.keysym.sym);
					world_handle_keydown(&world, event);
				break;

				case SDL_KEYUP:
				break;
					
			}
		}
		render(renderer, &world);
	}


	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	TTF_CloseFont(font);

	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
	return 0;
}
