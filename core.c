#include <libguile.h>
#include <SDL_image.h>
#include <gc.h>

#include "core.h"
#include "input.h"
#include "keytable.h"
#include "rgba.h"
#include "rect.h"
#include "point.h"
#include "graphics.h"
#include "sound.h"
#include "ttf.h"
#include "error.h"

playctx *scm_to_playctx(SCM ctx){
	return (playctx*) scm_to_long(ctx);
}

SCM scm_from_playctx(playctx *ctx){
	return scm_from_long((long) ctx);
}

/*static void playctx_finalizer(void *_ctx, void *unused){
	playctx *ctx = (playctx*) _ctx;
	if(ctx){
		if(ctx->keyboard.current){
			free(ctx->keyboard.current);
		}
		if(ctx->keyboard.last){
			free(ctx->keyboard.last);
		}
		if(ctx->renderer){
			SDL_DestroyRenderer(ctx->renderer);
		}
		if(ctx->window){
			SDL_DestroyWindow(ctx->window);
		}
	}
}*/

#define __SCM_FUNCTION__ "destroy-context"
static SCM destroy_context(SCM scm_ctx){
	playctx *ctx = scm_to_playctx(scm_ctx);
	if(ctx){
		if(ctx->keyboard.current){
			free(ctx->keyboard.current);
		}
		if(ctx->keyboard.last){
			free(ctx->keyboard.last);
		}
		if(ctx->renderer){
			SDL_DestroyRenderer(ctx->renderer);
		}
		if(ctx->window){
			SDL_DestroyWindow(ctx->window);
		}
		return SCM_BOOL_T;
	}
	else{
		return scm_errorstr("Invalid context");
	}
}
#undef __SCM_FUNCTION__

#define __SCM_FUNCTION__ "create-play-context"
static SCM create_context(SCM scm_title, SCM scm_width, SCM scm_height){
	int width = scm_to_int(scm_width);
	int height = scm_to_int(scm_height);
	char *title = scm_to_locale_string(scm_title);
	SDL_Window *window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
	if(window){
		SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		if(renderer){
			playctx *ctx = GC_malloc(sizeof(playctx));
			if(ctx){
				ctx->close_requested = false;
				ctx->keyboard.current = ctx->keyboard.last = NULL;
				ctx->mouse.x = ctx->mouse.y = 0;
				ctx->mouse.last = ctx->mouse.current = 0;
				ctx->window = window;
				ctx->renderer = renderer;
				// GC_register_finalizer(ctx, playctx_finalizer, NULL, 0, 0);
				free(title);
				return scm_from_playctx(ctx);
			}
			else{
				SDL_DestroyRenderer(renderer);
				SDL_DestroyWindow(window);
				free(title);
				return scm_errorstr("GC_malloc failed");
			}
		}
		else{
			SDL_DestroyWindow(window);
			free(title);
			return scm_errorstrf("SDL_CreateRenderer error: %s", SDL_GetError());
		}
	}
	else{
		free(title);
		return scm_errorstrf("SDL_CreateWindow error: %s", SDL_GetError());
	}
}
#undef __SCM_FUNCTION__

static SCM get_ticks(){
	return scm_from_uint32(SDL_GetTicks());
}

static SCM delay_ticks(SCM ticks){
	SDL_Delay(scm_to_uint32(ticks));
	return SCM_BOOL_T;
}

void init_play_core(void *unused){
	if(SDL_Init(0) != 0){
		fprintf(stderr, "SDL_Init failed with error: %s\n", SDL_GetError());
		return;
	}

	scm_c_define_gsubr("create-play-context", 3, 0, 0, create_context);
	scm_c_export("create-play-context", NULL);

	scm_c_define_gsubr("destroy-context", 1, 0, 0, destroy_context);
	scm_c_export("destroy-context", NULL);

	scm_c_define_gsubr("get-ticks", 0, 0, 0, get_ticks);
	scm_c_export("get-ticks");

	scm_c_define_gsubr("delay-ticks", 1, 0, 0, delay_ticks);
	scm_c_export("delay-ticks");

	init_keytable();
	init_input();
	init_graphics();
	init_sound();
	init_ttf();
}