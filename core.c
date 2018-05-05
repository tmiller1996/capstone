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
	// TODO rewrite this fn
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
	if(SDL_Init(0) != 0){
		fprintf(stderr, "SDL_Init failed with error: %s\n", SDL_GetError());
		return;
	}

	scm_c_define_gsubr("create-play-context", 3, 0, 0, create_context);
	scm_c_export("create-play-context", NULL);

	init_keytable();
	init_input();
	init_graphics();
	init_sound();
	init_ttf();
}