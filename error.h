#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
// #define UKI_ASSERT(cond, text, ...) if (!cond) {SDL_Log(text, __VA_ARGS__); exit(1);}


#define UKI_EXIT(win)\
win->is_running = false;\
SDL_DestroyRenderer(win->renderer);\
SDL_DestroyWindow(win->win);\
\
TTF_CloseFont(win->style.font);\
IMG_Quit();\
TTF_Quit();\
SDL_Quit();\
exit(-1);\



#define UKI_DEBUG(s, ...) SDL_Log(s __VA_ARGS__); SDL_Log("\n");
#define UKI_WARNING(...) UKI_DEBUG("warning: ", __VA_ARGS__)

#define UKI_ERROR(...) SDL_Log("ERROR: " __VA_ARGS__); exit(-1)


// #define UKI_ASSERT(expr, ...) if (expr) { UKI_ERROR( __VA_ARGS__); }
#define UKI_ASSERT(expr, err, ...) if (!(expr)) { UKI_ERROR(err, __VA_ARGS__); }
// #define UKI_ASSERT_ERR_CODE(expr, err, err_code) if (expr) { UKI_ERROR_ERR_CODE(err_code, err); }




// void uki_exit(WIN* win)
// {
	// win->is_running = false;
	// SDL_DestroyRenderer(win->renderer);
	// SDL_DestroyWindow(win->win);
	
	// TTF_CloseFont(win->style.font);
	// IMG_Quit();
	// TTF_Quit();
	// SDL_Quit();
	// exit(-1);
// }