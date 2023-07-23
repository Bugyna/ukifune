#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <wchar.h>

#include "core.h"
#include "sprite_map.h"

#define DEBUGGING 1

typedef struct
{
	ENTITY* e;
	int hp, mana;
	float dx, dy;

} PLAYER;


typedef struct
{
	WIN* win;
	WORLD world;
	PLAYER player;
	int state;
} GAME;



GAME game;

void update()
{
	win_event_handle(game.win);
	// game->player.e->abs_x += game->player.dx * DELTA_TIME;
	// game->player.e->abs_y += game->player.dy * DELTA_TIME;
	game.player.e->pos.x += game.player.dx;
	game.player.e->pos.y += game.player.dy;
}

u8 player_move_left BIND_FN_PARAMS
{
	if (game.player.dx <= 0 && game.player.dx > -10) {
		game.player.dx -= 0.7;
	}
	// SDL_Log("1 %d %d", game.player.e.tex.rect.x, *game.player.e.x);
	// ENTITY_SET_POS_REL(game.player.e.x, -10);
	// SDL_Log("2 %d %d", game.player.e.tex.rect.x, *game.player.e.x);
	return 0;
}

u8 player_move_right BIND_FN_PARAMS
{
	if (game.player.dx >= 0 && game.player.dx < 10) {
		game.player.dx += 0.7;
	}
	// ENTITY_SET_REL(game.player.e.x, 10);
	return 0;
}

u8 player_move_up BIND_FN_PARAMS
{
	if (game.player.dy == 0) game.player.dy = -2;
	// SDL_Log("1 %d %d", game.player.e.tex.rect.x, *game.player.e.x);
	// ENTITY_SET_POS_REL(game.player.e.x, -10);
	// SDL_Log("2 %d %d", game.player.e.tex.rect.x, *game.player.e.x);
	return 0;
}

u8 player_move_down BIND_FN_PARAMS
{
	if (game.player.dy == 0) game.player.dy = 2;
	// ENTITY_SET_REL(game.player.e.x, 10);
	return 0;
}

u8 player_rotate BIND_FN_PARAMS
{
	// SDL_Log("roate: %f", game.player.e->tex->angle);
	game.player.e->tex->angle += 22;
	return 0;
}

u8 player_reset_dx BIND_FN_PARAMS
{
	SDL_Log("reset");
	game.player.dx = 0;
	return 0;
}

u8 player_reset_dy BIND_FN_PARAMS
{
	SDL_Log("reset");
	game.player.dy = 0;
	return 0;
}

int main(int argc, char* argv[])
{
	game.win = win_create();

	world_init(&game.world);
	game.player.e = entity_create();
	TEXTURE* atlas = slice_map(game.win, "explosion1.png", 4, 4, 256, 256, 115, 115, 30, 35, 1.5);
	// ANIMATOR* a = create_anim(game.player.e, atlas, 16, 4, NULL);

	// entity_add_animator(game.player.e, a);
	entity_add_component(game.player.e, create_gravity(game.player.e, 10));
	entity_add_component(game.player.e, create_collider(game.player.e, (SDL_Rect){0,0,-20,-20}));
	game.player.e->tex = &atlas[9];
	ENTITY_LIST_APPEND(&game.win->render_list, game.player.e);
	// game.player.e = en;
	entity_move(game.player.e, 200, 0);

	// map_add_entity(&game.world.maps[0], &game.player.e);
	// win_set_render_queue(game.win, &game.world.maps[0], 0, 0, 0, 0);


	win_add_child(game.win, create_label(game.win, 1000, 100, "atak"));
	win_add_child(game.win, create_label(game.win, 1000, 120, "atak"));
	win_add_child(game.win, create_label(game.win, 1000, 140, "atak"));	
	win_add_child(game.win, create_label(game.win, 1000, 160, "atak"));
	win_add_child(game.win, create_label(game.win, 1000, 180, "atak"));
	win_add_child(game.win, create_label(game.win, 1000, 200, "atak"));
	win_add_child(game.win, create_label(game.win, 1000, 220, "NULL"));
	win_add_child(game.win, create_label(game.win, 1000, 240, "xxx"));


	win_bind(game.win, "<Escape>", win_close);
	win_bind(game.win, "<Escape>", win_close);
	win_bind(game.win, "[keyhold]a", player_move_left);
	win_bind(game.win, "[keyhold]d", player_move_right);
	win_bind(game.win, "[keyhold]w", player_move_up);
	win_bind(game.win, "[keyhold]s", player_move_down);
	win_bind(game.win, "[keyhold]r", player_rotate);
	win_bind(game.win, "[keyup]a", player_reset_dx);
	win_bind(game.win, "[keyup]d", player_reset_dx);
	win_bind(game.win, "[keyup]w", player_reset_dy);
	win_bind(game.win, "[keyup]s", player_reset_dy);
	// win_bind(&game.win, "<keypress>w", player_move_up);
	// win_bind(&game.win, "<keypress>s", player_move_down);
	// CAMERA camera;
	// game.p.entity = entity_create_from_texture(...);

	while (game.win->is_running)
	{
		change_widget_texture_int(game.win->children[1], game.win->mouse_x);
		change_widget_texture_int(game.win->children[2], game.win->mouse_y);
		change_widget_texture_int(game.win->children[3], FPS_COUNT);
		change_widget_texture_int(game.win->children[4], DELTA_TIME);
		change_widget_texture_text(game.win->children[5], game.win->focus->name);
		change_widget_texture_text(game.win->children[6], game.win->attention->name);
		if (game.win->lock != NULL) change_widget_texture_text(game.win->children[7], game.win->lock->name);
		else change_widget_texture_text(game.win->children[7], "NULL");
		char* tmp = keycode_list_pretty(game.win->keys_held);
		change_widget_texture_text(game.win->children[8], tmp);
		free(tmp);


		// if (FRAMES_ELAPSED % a->freq == 0) {
			// animator_trigger(a);
		// }
		update();
		win_render_default(game.win);
		cap_fps();
		// cap_fps_fixed(25);
	}

	return 0;
}


