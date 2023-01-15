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


int main () {
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

    
	uint32_t ticks = SDL_GetTicks();
	EVENT e;
	// e.type = 0;
	// (*win.fn[0])(&win, e);
	// SDL_Log("dd: %s %ld", "シシ", sizeof(WIDGET));

	// win_add_child(&win, create_label(&win, 200, 200, "test"));
	// win.children[1].label.tex = create_texture_from_image(&win, -100, 200, 200, 200, "test.jpg");
	// win_add_child(&win, create_label(&win, 400, 300, "áíýáí"));
	// win_add_child(&win, create_label(&win, 600, 300, "áíýáídwad"));
	// win_add_child(&win, create_text_input(&win, 500, 400, "atak"));
	// focus_set(&win, &win.children[4]);
	// focus_set_by_name(&win, "TEXT_INPUT4");
	// focus_set_to_last_child(&win);
	// SDL_SetTextInputRect(&win.children[4].text_input.tex.rect);
	// SDL_Log("focus: %s\n", focus_get(&win)->name);
	
	// win_set_render_fn(&win, win_render_test);
	// widget_bind(&win.children[3], "<keypress>", test_keypress);
	// win_bind(&win, "<mousemove>", test_mousemove);
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
                
				// case SDL_MOUSEMOTION:
                // win_handle_mouse_move(&win, event);
				// break;
                
				// case SDL_MOUSEBUTTONDOWN:
                // win_handle_mouse_button_down(&win, event);
				// break;
                
				// case SDL_KEYDOWN:
                // win_handle_keydown(&win, event);
				// break;
                
				// case SDL_TEXTINPUT:
                // win_handle_keydown(&win, event);
                // SDL_Log("textinput: %s %ld", event.text.text, strlen(event.text.text));
				// break;
                
				// case SDL_TEXTEDITING:
                // SDL_Log("textedit: %s %d %d", event.edit.text, event.edit.start, event.edit.length);
                // win_handle_keydown(&win, event);
				// break;
                
				case SDL_KEYUP:
				break;
			}
			win_render_default(&win);
		}
	}


	uki_destroy_win(&win);
	// uki_destroy_world();
	return 0;
}
