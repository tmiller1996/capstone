#include "core.h"

#include <libguile.h>
#include <SDL_image.h>

#include "input.h"
#include "keytable.h"

SCM poll_input(SCM scm_ctx){
	playctx *ctx = (playctx*) scm_to_long(scm_ctx);
	if(ctx){
		ctx_poll_input(ctx);
	}
	return SCM_BOOL_T;
}

SCM mouse_pos(SCM scm_ctx){
	playctx *ctx = (playctx*) scm_to_long(scm_ctx);
	if(ctx){
		return scm_cons(scm_from_int(ctx->mouse.x), scm_from_int(ctx->mouse.y));
	} else {
		// TODO error here
		return SCM_BOOL_F;
	}
}

SCM key_pressed(SCM scm_ctx, SCM scm_key){
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

SCM mouse_pressed(SCM scm_ctx, SCM scm_mbutton){

}

SCM close_requested(SCM scm_ctx){
	struct play_context *ctx = (struct play_context*) scm_to_long(scm_ctx);
	if(ctx){
		return ctx->close_requested ? SCM_BOOL_T : SCM_BOOL_F;
	}
	return SCM_BOOL_F;
}

SCM	create_context(SCM scm_title, SCM scm_width, SCM scm_height){
	char *title = scm_to_locale_string(scm_title);
	int width = scm_to_int(scm_width);
	int height = scm_to_int(scm_height);

	struct play_context *ctx = malloc(sizeof(struct play_context));
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
		free(ctx);
		free(title);
		// TODO error here
		return scm_from_long(0x0);
	}

	ctx->renderer = SDL_CreateRenderer(ctx->window, -1, SDL_RENDERER_ACCELERATED);
	if(!ctx->renderer){
		SDL_DestroyWindow(ctx->window);
		free(ctx);
		free(title);
		// TODO error here
		return scm_from_long(0x0);
	}
	free(title);
	return scm_from_long((long) ctx);
}

SCM destroy_context(SCM x){
	playctx *ctx = (playctx*) scm_to_long(x);
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
		free(ctx);
	}
	return SCM_BOOL_T;
}

SCM load_texture(SCM scm_ctx, SCM scm_path){
	playctx *ctx = (playctx*) scm_to_long(scm_ctx);
	if(!ctx){
		// TODO error
		return scm_from_long(0x0);
	}
	const char *path = scm_to_locale_string(scm_path);
	SDL_Surface *sfc = IMG_Load(path);
	if(!sfc){
		// TODO error
		return scm_from_long(0x0);
	}
	SDL_Texture *tex = SDL_CreateTextureFromSurface(ctx->renderer, sfc);
	if(!tex){
		SDL_FreeSurface(sfc);
		// TODO error
		return scm_from_long(0x0);
	}
	SDL_FreeSurface(sfc);
	return scm_from_long((long) tex);
}

SCM destroy_texture(SCM scm_tex){
	SDL_Texture *tex = (SDL_Texture*) scm_to_long(scm_tex);
	if(tex){
		SDL_DestroyTexture(tex);
	}
	return SCM_BOOL_T;
}

void init_play_core(void *unused){
#if 0
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
		// TODO error
	}
#endif
	init_keytable();

	scm_c_define_gsubr("create-play-context", 3, 0, 0, create_context);
	scm_c_export("create-play-context", NULL);

	scm_c_define_gsubr("destroy-play-context", 1, 0, 0, destroy_context);
	scm_c_export("destroy-play-context", NULL);

	scm_c_define_gsubr("load-texture", 2, 0, 0, load_texture);
	scm_c_export("load-texture", NULL);

	scm_c_define_gsubr("destroy-texture", 1, 0, 0, destroy_texture);
	scm_c_export("destroy-texture", NULL);

	scm_c_define_gsubr("poll-input", 1, 0, 0, poll_input);
	scm_c_export("poll-input", NULL);

	scm_c_define_gsubr("key-pressed", 2, 0, 0, key_pressed);
	scm_c_export("key-pressed", NULL);

	scm_c_define_gsubr("mouse-pressed", 2, 0, 0, mouse_pressed);
	scm_c_export("mouse-pressed", NULL);

	scm_c_define_gsubr("mouse-pos", 1, 0, 0, mouse_pos);
	scm_c_export("mouse-pos", NULL);

	scm_c_define_gsubr("close-requested", 1, 0, 0, close_requested);
	scm_c_export("close-requested", NULL);
}