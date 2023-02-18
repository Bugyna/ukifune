#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <wchar.h>

#include "core.h"

typedef struct PLAYER PLAYER;
struct PLAYER
{
	ENTITY e;
};

WIN win;
WORLD world;
SDL_Color xx = {150, 150, 150};
SDL_Rect r = {20, 20, 20, 20};

u8 test_drag(WIDGET* w, EVENT e, char* bind)
{
	// if (e.type == SDL_MOUSEMOTION) {
		// if (!e.button.button) return 0;
		
		// BIND* b = get_bind(w->binds, bind);
		BIND** b = get_binds_from_key(w->binds, bind);
		BIND* c = b[0];
		if (c == NULL) return 1;
		for (int i = 0; i < 5; i++)
		{
			if (b[i] == NULL) break;
			// SDL_Log("drag last event: %s", b[i]->bind);
		}
		// TEXTURE* t = get_widget_texture_ptr(w);/
		SDL_Rect* rect = get_widget_rect_ptr(w);
		// SDL_Log("aaaaa: %s %d %d %d\n", bind, t->rect.x, e.motion.x, c->rx);
		// SDL_Log("aaaaa: %s %d %d %d\n", bind, rect->x, e.motion.x, c->rx);
		// int tmp = t->rect.x;
		rect->x = e.motion.x - c->rx;
		rect->y = e.motion.y - c->ry;

		// t->rect.x = e.motion.x - c->rx;
		// t->rect.y = e.motion.y - c->ry;
		// t->rect.y = w->win_parent->mouse_y + (t->rect.y - w->win_parent->mouse_y);
	// }
	free(b);

	return 1;
}


u8 test_drag_2(WIDGET* w, EVENT e, char* bind)
{
	// if (e.type == SDL_MOUSEMOTION) {
		// if (!e.button.button) return 0;
		
		// BIND* b = get_bind(w->binds, bind);
		// SDL_Log("1: %s", bind);
		BIND** b = get_binds_from_key(w->binds, bind);
		// char** x = get_bind_names_from_key(bind);
		// SDL_Log("2: %s", bind);
		BIND* c = b[0];
		if (c == NULL) return 1;
		// for (int i = 0; i < 5; i++)
		// {
			// if (x[i] == NULL) break;
			// SDL_Log("[ %s ]   test drag 2: %s %s", w->name, x[i], b[i]->bind);
			// // SDL_Log("drag last event: %s %s", b[i]->bind, bind);
		// }
		// TEXTURE* t = get_widget_texture_ptr(w);/
		// SDL_Rect* rect = get_widget_rect_ptr(w);
		// SDL_Log("aaaaa: %s %d %d %d\n", bind, t->rect.x, e.motion.x, c->rx);
		// SDL_Log("aaaaa: %s %d %d %d\n", bind, rect->x, e.motion.x, c->rx);
		// int tmp = t->rect.x;

		// win.primitive_list.first->next = NULL;
		// SDL_Log("rect: %d %d %d %d", c->x, c->y, e.motion.x, e.motion.y);
		// SDL_Log("whatefuickc [ %s ] %s\n\n", w->name, c->bind);
		
		
		SDL_Rect r = {.x=c->x, .y=c->y, .w=e.motion.x-c->x, .h=e.motion.y-c->y};
		// SDL_Rect* r = malloc(sizeof(SDL_Rect));
		// r->x = c->x; r->y = c->y; r->w=e.motion.x-c->x; r->h = e.motion.y-c->y;
		PRIMITIVE* p = malloc(sizeof(PRIMITIVE));
		p->type = P_RECT;
		p->color = xx;
		p->r = r;
		SDL_Log("start: %p", p);
		// free(r);
		// *p = (PRIMITIVE){.type=P_RECT, .color=(SDL_Color){150, 150, 150}, .r=r};

		PRIMITIVE_LIST_APPEND(&w->win_parent->primitive_list, p);
		

		free(b);
		
		// SDL_Log("dddddd: %d %d", w->win_parent->primitive_list.last->p.r.x, w->win_parent->primitive_list.last->p.r.y);
		// SDL_SetRenderDrawColor(w->win_parent->renderer, 255, 0, 75, 255);
		// SDL_RenderDrawRect(w->win_parent->renderer, &r);
		// SDL_RenderPresent(w->win_parent->renderer);
		// rect->x = e.motion.x - c->rx;
		// rect->y = e.motion.y - c->ry;

		// t->rect.x = e.motion.x - c->rx;
		// t->rect.y = e.motion.y - c->ry;
		// t->rect.y = w->win_parent->mouse_y + (t->rect.y - w->win_parent->mouse_y);
	// }

	return 1;
}

u8 test_drag_x(WIDGET* w, EVENT e, char* bind)
{
	BIND** b = get_binds_from_key(w->binds, bind);
	BIND* c = b[0];
	if (c == NULL) return 1;
	SDL_Rect* rect = get_widget_rect_ptr(w);
	rect->x = e.motion.x - c->rx;

	free(b);

	return 1;
}

u8 empty_test(WIDGET* w, EVENT e, char* bind)
{
	SDL_Log("empty test");
	return 1;
}

int main (int argc, char* argv[]) {
	win_init(&win);
	world_init(&world);
	
	// TEXTURE tex = create_texture_from_image(win.renderer, 200, 200, 200, 200, "test.jpg");
	// ENTITY en = entity_create_from_texture((VEC3){0, 0, 0}, tex);
	
	// entity_init(&en);
	// entity_create_from_texture(&en
	// map_add_renderable_entity(&world.maps[0], en);

	
	// win.render_fn = win_render_default;
	// SDL_Log("aaaaaa: %d", world.render_list[0]->components[0].type);
	
	SDL_Event event;
	bool moving = false;
	// parse_bind_key("<mouse_right><mouse_move>");

	
	uint32_t ticks = SDL_GetTicks();
	EVENT e;
	// e.type = 0;
	// (*win.fn[0])(&win, e);
	// SDL_Log("dd: %s %ld", "シシ", sizeof(WIDGET));

	win_add_child(&win, create_label(&win, 200, 200, "test"));
	win.children[1].label.tex = create_texture_from_image(&win, -100, 200, 200, 200, "test.jpg");
	win_add_child(&win, create_label(&win, 400, 300, "áídwaáí"));
	win_add_child(&win, create_label(&win, 600, 300, "áídwaídwad"));
	win_add_child(&win, create_text_input(&win, 500, 400, "atak"));
	win_add_child(&win, create_label(&win, 1000, 100, "atak"));
	win_add_child(&win, create_label(&win, 1000, 120, "atak"));
	win_add_child(&win, create_label(&win, 1000, 140, "atak"));	
	win_add_child(&win, create_label(&win, 1000, 160, "atak"));
	win_add_child(&win, create_label(&win, 1000, 180, "atak"));
	win_add_child(&win, create_label(&win, 1000, 200, "atak"));
	win_add_child(&win, create_label(&win, 1000, 220, "NULL"));
	// win_add_child(&win, create_label(&win, 1000, 180, "atak"));
	focus_set(&win, &win.children[0]);
	attention_set(&win, &win.children[0]);
	// focus_set(&win, &win.children[3]);



	// change_widget_texture_text(&win.children[5], "dwa");
	// focus_set_by_name(&win, "TEXT_INPUT4");
	// focus_set_to_last_child(&win);
	// SDL_SetTextInputRect(&win.children[4].text_input.tex.rect);
	// SDL_Log("focus: %s\n", focus_get(&win)->name);
	
	// win_set_render_fn(&win, win_render_test);
	// widget_bind(&win.children[3], "<keypress>", test_keypress);
	// widget_bind(&win.children[0], "<mouse_right_drag>", win_close);
	// widget_bind(&win.children[3], "z<mouse_move>", test_drag);
	// widget_bind(&win.children[3], "z<mouse_right><mouse_move>", test_drag_x);
	// widget_bind(&win.children[3], "<z>", test_drag);
	widget_bind(&win.children[0], "<Escape>", win_close);
	widget_bind(&win.children[3], "<mouse_right><mouse_move>", test_drag_x);
	widget_bind(&win.children[3], "<mouse_middle><mouse_move>", test_drag);
	widget_bind(&win.children[0], "<mouse_left><mouse_move>", test_drag_2);
	widget_bind(&win.children[0], "z<mouse_move>", test_drag_2);
	widget_bind(&win.children[0], "a<mouse_move>", empty_test);
	// SDL_Log("hash mouse move: %d", hash("<mouse_move>") % win.children[3].binds.size);
	// SDL_Log("hash mouse middle move: %d", hash("<mouse_middle><mouse_move>") % win.children[3].binds.size);
	widget_bind(&win.children[1], "<mouse_middle><mouse_move>", test_drag);
	// win_bind(&win, "<mousemove>", test_mousemove);
	// exit(1);

	// char* bind = malloc(300);
	char* bind;
	// int offset = 0;
	// BIND* x = get_bind(win.children[0].binds, "<mouse_right><mouse_move>");
	// if (x != NULL && x->bind != NULL)
		// SDL_Log("dwa: %s", x->bind);

	// x = get_bind(win.children[0].binds, "<mouse_middle><mouse_move>");
	// if (x != NULL && x->bind != NULL)
		// SDL_Log("dwa: %s", x->bind);
	// exit(1);

	SDL_StartTextInput();
	while (win.is_running) {
		ticks = SDL_GetTicks();
		// if (win.focus->name != NULL) change_widget_texture_text(&win.children[9], win.focus->name);
		win.children[5].label.tex.rect.x+=1*DELTA_TIME;
		if (win.children[5].label.tex.rect.x > 1200) win.children[5].label.tex.rect.x = 0;
		// change_widget_texture_text(&win.children[5], "dwa");
		while (SDL_PollEvent(&event)) {
			// PRIMITIVE_LIST_POP(&win.primitive_list);
			// SDL_Log("event: %d", event.type);
			switch (event.type) {
				case SDL_QUIT:
					win.is_running = false;
				break;

				case SDL_WINDOWEVENT:
					win_handle_window_event(&win, event);
				break;

				case SDL_MOUSEBUTTONDOWN:
					// SDL_Log("a:: %d\n", event.button.button);
					win_handle_mouse_button_down(&win, event);
				break;

				case SDL_MOUSEBUTTONUP:
					// SDL_Log("a:: %d\n", event.button.button);
					win_handle_mouse_button_up(&win, event);
				break;

				case SDL_MOUSEMOTION:
					PRIMITIVE_LIST_POP(&win.primitive_list);
					bind = win_handle_mouse_move(&win, event);
					free(bind);
					// SDL_Log("a:: %d\n", event.button.button);
				break;
				
				case SDL_KEYDOWN:
					// if (win.focus == &win.children[3]) win.focus = &win.children[0];
					// else win.focus = &win.children[3];
					
					win_handle_keydown(&win, event);
					// SDL_Log("a:: %d\n", event.key.keysym.sym);
				break;
				
				// case SDL_TEXTINPUT:
					// win_handle_keydown(&win, event);
					// // SDL_Log("textinput: %s %ld", event.text.text, strlen(event.text.text));
				// break;
				
				// case SDL_TEXTEDITING:
					// // SDL_Log("textedit: %s %d %d", event.edit.text, event.edit.start, event.edit.length);
					// win_handle_keydown(&win, event);
				// break;
				
				// case SDL_KEYUP:
					// win_handle_keyup(&win, event);
				// break;
			}
		}
		change_widget_texture_int(&win.children[5], win.mouse_x);
		change_widget_texture_int(&win.children[6], win.mouse_y);
		change_widget_texture_int(&win.children[7], FPS_COUNT);
		change_widget_texture_int(&win.children[8], DELTA_TIME);
		change_widget_texture_text(&win.children[9], win.focus->name);
		change_widget_texture_text(&win.children[10], win.attention->name);
		if (win.lock != NULL) change_widget_texture_text(&win.children[11], win.lock->name);
		else change_widget_texture_text(&win.children[11], "NULL");
		win_render_default(&win);
		cap_fps(&ticks);
		// win.lock = NULL;
		// cap_fps_fixed(&ticks, 1000/120);
	}


	uki_close(&win);
	return 0;
}
