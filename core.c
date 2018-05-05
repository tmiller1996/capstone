#include "core.h"

#include <libguile.h>
#include <SDL_image.h>
#include <gc.h>

#include "input.h"
#include "keytable.h"
#include "rgba.h"
#include "rect.h"
#include "point.h"
#include "graphics.h"
#include "sound.h"
#include "ttf.h"

extern playctx *scm_to_playctx(SCM ctx){
	return (playctx*) scm_to_long(ctx);
}

extern SCM scm_from_playctx(playctx *ctx){
	return scm_from_long((long) ctx);
}

static SCM poll_input(SCM scm_ctx){
	playctx *ctx = (playctx*) scm_to_long(scm_ctx);
	if(ctx){
		ctx_poll_input(ctx);
	}
	return SCM_BOOL_T;
}

static SCM mouse_pos(SCM scm_ctx){
	playctx *ctx = (playctx*) scm_to_long(scm_ctx);
	if(ctx){
		return scm_cons(scm_from_int(ctx->mouse.x), scm_from_int(ctx->mouse.y));
	} else {
		// TODO error here
		return SCM_BOOL_F;
	}
}

static SCM key_pressed(SCM scm_ctx, SCM scm_key){
	playctx *ctx = (playctx*) scm_to_long(scm_ctx);
	if(ctx){
		SDL_Keycode key = (SDL_Keycode) scm_to_uint8(scm_key);
		SDL_Scancode scan = SDL_GetScancodeFromKey(key);
		bool current = ctx->keyboard.current ? ctx->keyboard.current[scan] : false;
		bool last = ctx->keyboard.last ? ctx->keyboard.last[scan] : false;
		return current && !last ? SCM_BOOL_T : SCM_BOOL_F;
	} else {
		return SCM_BOOL_F;
	}
}

static SCM key_down(SCM scm_ctx, SCM scm_key){
	playctx *ctx = (playctx*) scm_to_long(scm_ctx);
	if(ctx){
		SDL_Keycode key = (SDL_Keycode) scm_to_uint8(scm_key);
		SDL_Scancode scan = SDL_GetScancodeFromKey(key);
		bool current = ctx->keyboard.current ? ctx->keyboard.current[scan] : false;
		return current ? SCM_BOOL_T : SCM_BOOL_F;
	}
	return SCM_BOOL_F;
}

static SCM mouse_pressed(SCM scm_ctx, SCM scm_mbutton){
	// TODO
	return SCM_BOOL_F;
}

static SCM close_requested(SCM scm_ctx){
	playctx *ctx = scm_to_playctx(scm_ctx);
	if(ctx){
		return ctx->close_requested ? SCM_BOOL_T : SCM_BOOL_F;
	}
	return SCM_BOOL_F;
}

static void playctx_finalizer(void *_ctx, void *unused){
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
}

static SCM create_context(SCM scm_title, SCM scm_width, SCM scm_height){
	char *title = scm_to_locale_string(scm_title);
	int width = scm_to_int(scm_width);
	int height = scm_to_int(scm_height);

	playctx *ctx = GC_malloc(sizeof(playctx));
	GC_register_finalizer(ctx, playctx_finalizer, NULL, 0, 0);
	if(!ctx){
		free(title);
		// TODO error here
		return scm_from_long(0x0);
	}

	ctx->close_requested = false;
	ctx->keyboard.current = ctx->keyboard.last = NULL;
	ctx->mouse.x = ctx->mouse.y = 0;
	ctx->mouse.current = ctx->mouse.last = 0x0;

	ctx->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
	if(!ctx->window){
		GC_free(ctx);
		free(title);
		// TODO error here
		return scm_from_long(0x0);
	}

	ctx->renderer = SDL_CreateRenderer(ctx->window, -1, SDL_RENDERER_ACCELERATED);
	if(!ctx->renderer){
		SDL_DestroyWindow(ctx->window);
		ctx->window = NULL;
		GC_free(ctx);
		free(title);
		// TODO error here
		return scm_from_long(0x0);
	}
	free(title);
	return scm_from_long((long) ctx);
}

void init_play_core(void *unused){
#if 0
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
		// TODO error
	}
#endif
	init_keytable();
	init_input();
	init_graphics();
	init_sound();
	init_ttf();

	scm_c_define_gsubr("create-play-context", 3, 0, 0, create_context);
	scm_c_export("create-play-context", NULL);

	scm_c_define_gsubr("poll-input", 1, 0, 0, poll_input);
	scm_c_export("poll-input", NULL);

	scm_c_define_gsubr("key-pressed", 2, 0, 0, key_pressed);
	scm_c_export("key-pressed", NULL);

	scm_c_define_gsubr("key-down", 2, 0, 0, key_down);
	scm_c_export("key-down", NULL);

	scm_c_define_gsubr("mouse-pressed", 2, 0, 0, mouse_pressed);
	scm_c_export("mouse-pressed", NULL);

	scm_c_define_gsubr("mouse-pos", 1, 0, 0, mouse_pos);
	scm_c_export("mouse-pos", NULL);

	scm_c_define_gsubr("close-requested", 1, 0, 0, close_requested);
	scm_c_export("close-requested", NULL);
}